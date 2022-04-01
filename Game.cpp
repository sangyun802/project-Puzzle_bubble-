#include "Game.h"

using namespace std;

#define WIDTH 600
#define HEIGHT 800
#define boundaryX WIDTH/2
#define boundaryY (HEIGHT-200)/2  //game space boundary
#define radius 25
#define PI 3.14159265358979323846264338

Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);
vector<SolidSphere> spheres;
CollisionHandling collisionHandler;
CollisionDetection collison;
Cannon* cyl = new Cannon;
Timebar T;
int score = 0; //initialize score
double width, height; //texture size

static GLuint textureID;
GLubyte* textureData;
int imageWidth, imageHeight;

//Text
void draw_characters_white(void* font, const char* c, float x, float y) {
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);
	for (size_t i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
}

void draw_characters_purple(void* font, const char* c, float x, float y) {
	glColor3f(0.8, 0.3, 0.8);
	glRasterPos2f(x, y);
	for (size_t i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
}

//�ֺ� 6�� �� �߿��� ������ ���� �� ã�Ƽ� remove�� true�� �ٲ�
void ColorHandling(SolidSphere& sph) {
	SamePosition sp;
	vector<SolidSphere>::iterator it;

	sph.setRemove(true);
	for (int i = 0; i < 6; i++) {
		sp.setpos(sph.getSurround()[i]);
		it = find_if(spheres.begin(), spheres.end(), sp);
		if (it != spheres.end()) {
			//���� �� ���� ã���� �� ���� ���� �ٽ� ColorHandling
			if ((*it).getMTL() == sph.getMTL() && (*it).getRemove() == false) {
				ColorHandling((*it));
			}
		}
	}

}

//drop�� true�� ���� �ֺ� ���鵵 drop�� true�� �ٲ���
void DropHandling2(SolidSphere& sph) {
	SamePosition sp;
	vector<SolidSphere>::iterator it;
	for (int i = 0; i < 6; i++) {
		sp.setpos(sph.getSurround()[i]);
		it = find_if(spheres.begin(), spheres.end(), sp);
		if (it != spheres.end() && (*it).getDrop() == false) {
			(*it).setDrop(true);
			DropHandling2(*it);
		}
	}
}

//���� ���� �پ� �ִ� ������ drop�� true�� �ٲٰ� �� �ֺ� 6���� ���鿡 DropHandling2
void DropHandling() {
	Vector3 center_pos;
	SamePosition sp;
	vector<SolidSphere>::iterator it;
	for (int i = 0; i < 12; i++) {
		center_pos.setXYZ(-boundaryX + radius + 2 * radius * i, boundaryY - radius, 0);
		sp.setpos(center_pos);
		it = find_if(spheres.begin(), spheres.end(), sp);
		if (it != spheres.end() && (*it).getDrop()== false) {
			(*it).setDrop(true);
			DropHandling2(*it);
		}
	}
}

//�������� �� ���� -boundaryY�� ���� ��� Game End
bool EndDetection(const SolidSphere& sph) {
	if (sph.getCenter()[1] - radius > -boundaryY)
		return false;
	if (sph.getCenter()[1] + radius < -boundaryY)
		return false;
	if (dotProduct(sph.getVelocity(), sph.getVelocity()) != 0)
		return false;
	
	return true;
}

//Texture
FIBITMAP* createBitMap(char const* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1) {
		exit(-1);
	}

	if (format == FIF_UNKNOWN) {
		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format)) {
			exit(-1);
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32) {
		bitmap32 = bitmap;
	}
	else {
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	return bitmap32;
}

void init() {
	//Light
	light.setAmbient(0.5, 0.5, 0.5, 1.0);
	light.setDiffuse(0.7, 0.7, 0.7, 1.0);
	light.setSpecular(1.0, 1.0, 1.0, 1.0);
	width = height = 500;

	//Texture
	FIBITMAP* bitmap32 = createBitMap("sha.png");
	imageWidth = FreeImage_GetWidth(bitmap32);
	imageHeight = FreeImage_GetHeight(bitmap32);
	textureData = FreeImage_GetBits(bitmap32);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);
	
	//create initial and next spheres
	srand((unsigned int)time(0));
	SolidSphere initial_sph(radius, 50, 50);
	initial_sph.setCenter(0, -355, 0);
	initial_sph.setVelocity(0, 0, 0);
	Material mtl;
	initial_sph.setMTL(mtl);
	spheres.push_back(initial_sph);

	SolidSphere initial_next_sph(radius, 50, 50);
	initial_next_sph.setCenter(-150, -355, 0);
	initial_next_sph.setVelocity(0, 0, 0);
	Material mtl_2;
	initial_next_sph.setMTL(mtl_2);
	spheres.push_back(initial_next_sph);
}

void idle() {
	int size = spheres.size();
	int now = size - 2;
	int next = size - 1;
	float before_vy = spheres[now].getVelocity()[1];

	//����, ������ ���� �ε����� ƨ��
	if (spheres[now].getCenter()[0] + radius >= boundaryX ||
		spheres[now].getCenter()[0] - radius <= -boundaryX) {
		spheres[now].setVelocity(-spheres[now].getVelocity()[0], spheres[now].getVelocity()[1], spheres[now].getVelocity()[2]);
	}

	if (spheres[now].getCenter()[1] + radius >= boundaryY) {
		//���� ���� 12���� �ٵ���
		for (float i = -5; i <= 6; i++)
		{
			if (spheres[now].getCenter()[0] <= i * boundaryX / 6 && spheres[now].getCenter()[0] >= (i - 1) * boundaryX / 6)
			{
				spheres[now].setCenter((2 * i - 1) * radius, boundaryY - radius, 0);
				spheres[now].setVelocity(0, 0, 0);
				spheres[now].setSurround(spheres[now].getCenter());
				break;
			}
		}
	}

	for (auto& sph : spheres) {
		sph.move();
	}

	//���� ��� ���� Timebar �ʱ�ȭ
	if (spheres[now].getVelocity()[1] != 0)
		T.setTimebar(250);

	//collisionHandling �ϰ�, ��� ������ remove, drop�� false�� �ʱ�ȭ
	for (int i = 0; i < now; i++) {
		collisionHandler(spheres[i], spheres[now]);
		spheres[i].setRemove(false);
		spheres[i].setDrop(false);
	}

	//�� ���� ���� ��� 
	if (spheres[now].getVelocity()[1] == 0 && spheres[now].getVelocity()[1] != before_vy) {
		//Remove
		ColorHandling(spheres[now]);
		Remove rm;
		vector<SolidSphere>::const_iterator it;
		if (count_if(spheres.begin(), spheres.end(), rm) >= 3) {
			while ((it = find_if(spheres.begin(), spheres.end(), rm)) != spheres.end()) {
				score += 10;
				spheres.erase(it);
			}
		}

		//Drop
		if (spheres.size() >= 2) {
			DropHandling();
			Drop dr;
			for (size_t i = 0; i < spheres.size() - 1; i++) {
				if (dr(spheres[i]))
					spheres[i].setVelocity(0, -6, 0);
			}
		}

		//next���� now�� �ű��, next�� ���ο� �� ����
		if (spheres.size() >= 2)
		{
			if (EndDetection(spheres[spheres.size() - 2]) == false) {
				spheres[spheres.size() - 1].setCenter(0, -355, 0);
				SolidSphere new_sph(radius, 50, 50);
				new_sph.setCenter(-150, -355, 0);
				new_sph.setVelocity(0, 0, 0);
				Material mtl;
				new_sph.setMTL(mtl);
				spheres.push_back(new_sph);
			}
		}
		else {
			spheres[spheres.size() - 1].setCenter(0, -355, 0);
			SolidSphere new_sph(radius, 50, 50);
			new_sph.setCenter(-150, -355, 0);
			new_sph.setVelocity(0, 0, 0);
			Material mtl;
			new_sph.setMTL(mtl);
			spheres.push_back(new_sph);
		}
	}

	//drop�� ������ -boundary�� ������ erase
	for (vector<SolidSphere>::iterator it = spheres.begin(); it != spheres.end();) {
		if ((*it).getVelocity()[1] == -6 && (*it).getCenter()[1] - radius <= -boundaryY)
		{
			score += 20;
			it = spheres.erase(it);
		}
		else
			++it;
	}
	
	glutPostRedisplay();
}

void drawQuadWithTexturing() {
	glPushMatrix();
	glTranslatef(0.0, 0.0, -4.5);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(-0.5, -0.36, 0.0);
	glTexCoord2f(0, 1);		glVertex3f(-0.5, 0.36, 0.0);
	glTexCoord2f(1, 1);		glVertex3f(0.5, 0.36, 0.0);
	glTexCoord2f(1, 0);		glVertex3f(0.5, -0.36, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();
}

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(12.0, (GLfloat)width / (GLfloat)height, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawQuadWithTexturing();

	glutSwapBuffers();
}

void renderScene() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(12.0, (GLfloat)width / (GLfloat)height, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawQuadWithTexturing();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX, boundaryX, -HEIGHT / 2, HEIGHT / 2, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//draw line
	glColor3f(1, 1, 1);
	glLineWidth(6.0f);
	glBegin(GL_LINES);
	//bottom
	glVertex2f(-boundaryX, -300);
	glVertex2f(boundaryX, -300);
	//top
	glVertex2f(-boundaryX, 300);
	glVertex2f(boundaryX, 300);

	glEnd();

	// ���� ǥ��
	char ntoc[(((sizeof score) * CHAR_BIT) + 2) / 3 + 2];
	sprintf_s(ntoc, "%d", score);
	draw_characters_white(GLUT_BITMAP_TIMES_ROMAN_24, "SCORE", -290.0, 370.0);
	draw_characters_white(GLUT_BITMAP_TIMES_ROMAN_24, ntoc, -280.0, 330.0);
	draw_characters_white(GLUT_BITMAP_TIMES_ROMAN_24, "Points", -200.0, 330.0);
	draw_characters_white(GLUT_BITMAP_TIMES_ROMAN_24, "TIME", 50.0, 370.0);
	draw_characters_white(GLUT_BITMAP_TIMES_ROMAN_24, "Next :", -290.0, -360.0);


	T.draw();
	if (T.getTimebar() <= 50)
	{
		spheres[spheres.size() - 2].setVelocity(2 * sin(cyl->getangle() * PI / 180.0), 2 * cos(cyl->getangle() * PI / 180.0), 0);
		glutIdleFunc(idle);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	light.draw();

	for (auto sph : spheres) {
		sph.draw();
	}

	cyl->draw();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	//������ ���� ���
	if (EndDetection(spheres[spheres.size() - 2])) {
			draw_characters_purple(GLUT_BITMAP_TIMES_ROMAN_24, "Game End", -60, 0);
			draw_characters_purple(GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ESC to exit", -100, -30);
			
			//time bar ���߱�
			T.setVelocity(0);
	}
		glutSwapBuffers();
}

void processSpecialKeys(int key, int x, int y) {
	//CANNON ����
	float ang = cyl->getangle();
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (ang <= 80)
			ang = ang + 3;
		cyl->setangle(ang);
		break;
	case GLUT_KEY_LEFT:
		if (ang >= -80)
			ang = ang - 3;
		cyl->setangle(ang);
		break;
	case GLUT_KEY_UP:
		if (dotProduct(spheres[spheres.size() - 2].getVelocity(), spheres[spheres.size() - 2].getVelocity()) == 0) {
			spheres[spheres.size() - 2].setVelocity(2 * sin(cyl->getangle() * PI / 180.0), 2 * cos(cyl->getangle() * PI / 180.0), 0);
			glutIdleFunc(idle);
		}
		break;
	}
}

void processNormalKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		//���� ���� ��� ESC Ű ������ exit
		if (EndDetection(spheres[spheres.size() - 2])) {
			exit(0);
		}
		
		break;
	case 32:
		//�����̽��� ������ �� ����
		if (dotProduct(spheres[spheres.size() - 2].getVelocity(), spheres[spheres.size() - 2].getVelocity()) == 0) {
			spheres[spheres.size() - 2].setVelocity(2*sin(cyl->getangle() * PI / 180.0), 2*cos(cyl->getangle() * PI / 180.0), 0);
			glutIdleFunc(idle);
		}
		break;
	}
}
	

