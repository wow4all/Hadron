#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <hadron/hadron.hpp>

#define rand(n, m) sf::Randomizer::Random(n, m)

GLuint MakeCubeList()
{
	GLuint list = glGenLists(1);

	/* Left face */
	// Bottom left back
	const GLfloat v1[] = { -1.0f, -1.0f, 1.0f };
	// Bottom left front
	const GLfloat v2[] = { -1.0f, -1.0f, -1.0f };
	// Top left front
	const GLfloat v3[] = { -1.0f, 1.0f, -1.0f };
	// Top left back
	const GLfloat v4[] = { -1.0f, 1.0f, 1.0f };

	/* Back face */
	// Top left back
	// ( v4 )
	// Top right back
	const GLfloat v5[] = { 1.0f, 1.0f, 1.0f };
	// Bottom right back
	const GLfloat v6[] = { 1.0f, -1.0f, 1.0f };
	// Bottom left back
	// ( v1 )

	/* Bottom face */
	// Bottom left back
	// ( v1 )
	// Bottom right back
	// ( v6 )
	// Bottom right front
	const GLfloat v7[] = { 1.0f, -1.0f, -1.0f };
	// Bottom left front
	// ( v2 )

	/* Front face */
	// Bottom left front
	// ( v2 )
	// Bottom right front
	// ( v7 )
	// Top right front
	const GLfloat v8[] = { 1.0f, 1.0f, -1.0f };
	// Top left front
	// ( v3 )

	/* Top face */
	// Top left front
	// ( v3 )
	// Top right front
	// ( v8 )
	// Top right back
	// ( v5 )
	// Top left back
	// ( v4 )

	glNewList(list, GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
			// Left face
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glVertex3fv(v3);
			glVertex3fv(v4);

			// Back face
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3fv(v4);
			glVertex3fv(v5);
			glVertex3fv(v6);
			glVertex3fv(v1);

			// Bottom face
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3fv(v1);
			glVertex3fv(v6);
			glVertex3fv(v7);
			glVertex3fv(v2);

			// Front face
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3fv(v2);
			glVertex3fv(v7);
			glVertex3fv(v8);
			glVertex3fv(v3);

			// Top face
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3fv(v3);
			glVertex3fv(v8);
			glVertex3fv(v5);
			glVertex3fv(v4);
		glEnd();
	glEndList();

	return list;
}

int main()
{
	// Convenience
	using Hadron::real;

	// Generic stuff
	const int W = 1024;
	const int H = 768;

	sf::RenderWindow window(sf::VideoMode(W, H, 32), "Hadron - Particles", sf::Style::Close);

	// Setup OpenGL bits
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(85.0f, W / H, 0.01f, 1000.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	const GLfloat LIGHT_POS[] = { 0.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);

	Hadron::Particle a, b;
	a.SetPosition(Hadron::Vector3<real>((real)0.0, (real)0.0, (real)0.0));
	a.SetVelocity((real)rand(-30.0f, 30.0f), (real)rand(-30.0f, 30.0f), (real)rand(-30.0f, 30.0f));
	a.SetAcceleration(Hadron::Vector3<real>::HIGH_GRAVITY);
	a.SetAlive(true);
	a.SetMass((real)200.0);

	b.SetPosition(Hadron::Vector3<real>((real)0.0, (real)0.0, (real)0.0));
	//b.SetVelocity((real)rand(-30.0f, 30.0f), (real)rand(-30.0f, 30.0f), (real)rand(-30.0f, 30.0f));
	b.SetAcceleration(Hadron::Vector3<real>::ZERO);
	b.SetAlive(true);
	b.SetMass((real)200.0);

	Hadron::ParticleForceRegistry registry;
	Hadron::ParticleSpring aSpring(&b, (real)3000.0, (real)20.0);
	Hadron::ParticleSpring bSpring(&a, (real)3000.0, (real)20.0);

	Hadron::ParticleDrag drag((real)1.0, (real)2.0);

	registry.Add(&a, &aSpring);
	//registry.Add(&b, &bSpring);
	registry.Add(&a, &drag);

	GLuint LIST_CUBE = MakeCubeList();

	double time = 0.0;

	while(window.IsOpened())
	{
		sf::Event e;
		while(window.GetEvent(e))
		{
			switch(e.Type)
			{
			case(sf::Event::Closed):
				window.Close();
				break;

			case(sf::Event::KeyPressed):
				if(e.Key.Code == sf::Key::Space) a.ApplyForce((real)0.0, a.GetMass() * (real)400000.0, (real)0.0);
			}
		}

		double frameTime = window.GetFrameTime();
		time += frameTime;

		registry.ApplyForces(frameTime);
		a.Update(frameTime);
		b.Update(frameTime);

		if(a.GetY() < (real)-30.0) a.SetVelocityY(abs(a.GetVelocity().y * -0.9));
		if(b.GetY() < (real)-30.0) b.SetVelocityY(abs(b.GetVelocity().y * -0.9));

		if(a.GetX() < (real)-30.0 || a.GetX() > (real)30.0) a.SetVelocityX(a.GetVelocity().x * -0.9);
		if(b.GetX() < (real)-30.0 || b.GetX() > (real)30.0) b.SetVelocityX(b.GetVelocity().x * -0.9);

		if(a.GetZ() < (real)-30.0 || a.GetZ() > (real)30.0) a.SetVelocityZ(a.GetVelocity().z * -0.9);
		if(b.GetZ() < (real)-30.0 || b.GetZ() > (real)30.0) b.SetVelocityZ(b.GetVelocity().z * -0.9);

		window.Clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Move the camera a little
		glTranslatef(0.0f, 0.0, -50.0f);

		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-31.0f, -31.0f, -31.0f);
		glVertex3f(-31.0f, -31.0f, 31.0f);
		glVertex3f(31.0f, -31.0f, 31.0f);
		glVertex3f(31.0f, -31.0f, -31.0f);
		glEnd();

		glPushMatrix();
		glTranslatef(a.GetX(), a.GetY(), a.GetZ());
		glCallList(LIST_CUBE);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(b.GetX(), b.GetY(), b.GetZ());
		glCallList(LIST_CUBE);
		glPopMatrix();

		glDisable(GL_LIGHTING);
		glColor3f(0.5f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex3f(a.GetX(), a.GetY(), a.GetZ());
		glVertex3f(b.GetX(), b.GetY(), b.GetZ());
		glEnd();
		glEnable(GL_LIGHTING);

		window.Display();
	}

	// Clean up
	glDeleteLists(LIST_CUBE, 1);
}