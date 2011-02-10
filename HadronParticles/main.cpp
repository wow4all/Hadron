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

	const int MAX_PARTICLES = 1000;
	Hadron::Particle particle[MAX_PARTICLES];

	Hadron::ParticleGravitation gravitor;
	gravitor.SetGravityPosition((real)0.0, (real)0.0, (real)0.0);

	Hadron::ParticleForceRegistry registry;
	
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		registry.Add(&particle[i], &gravitor);
	}

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
				if(e.Key.Code == sf::Key::Space)
				{
					int index = sf::Randomizer::Random(0, MAX_PARTICLES - 1);
					particle[index].SetPosition(Hadron::Vector3<real>(rand(-10.0f, 10.0f), rand(-10.0f, 10.0f), rand(-10.0f, 10.0f)));
					particle[index].SetVelocity(Hadron::Vector3<real>(rand(-30.0f, 30.0f), rand(-30.0f, 30.0f), rand(-30.0f, 30.0f)));
					particle[index].SetAcceleration(Hadron::Vector3<real>::ZERO);
					particle[index].SetMass((real)1.0);
					particle[index].SetAlive(true);
				}
			}
		}

		double frameTime = window.GetFrameTime();
		time += frameTime;

		registry.ApplyForces(frameTime);
		for(int i = 0; i < MAX_PARTICLES; i++)
		{
			particle[i].Update((real)frameTime);
		}

		window.Clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Move the camera a little
		glTranslatef(0.0f, 0.0, -50.0f);
		glRotatef(time * 30.0f, 0.0f, 1.0f, 0.0f);
		
		for(int i = 0; i < MAX_PARTICLES; i++)
		{
			glPushMatrix();

			glTranslatef(particle[i].GetX(), particle[i].GetY(), particle[i].GetZ());

			glCallList(LIST_CUBE);

			glPopMatrix();
		}

		window.Display();
	}

	// Clean up
	glDeleteLists(LIST_CUBE, 1);
}