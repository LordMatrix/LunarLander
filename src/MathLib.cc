#include "../include/MathLib.h"

void MathLib::initVec2(Vec2 *v, float x, float y) {
	(*v).x = x;
	(*v).y = y;
}

void MathLib::initVec3(Vec3 *v, float x, float y, float z) {
	(*v).x = x;
	(*v).y = y;
	(*v).z = 1;
}

void MathLib::initPoint2(Point2 *p, float x, float y) {
	(*p).x = x;
	(*p).y = y;
}

void MathLib::initMat2(Mat2 *m, float w, float x, float y, float z) {
	(*m)[0][0] = w;
	(*m)[0][1] = x;
	(*m)[1][0] = y;
	(*m)[1][1] = z;
}

void MathLib::initMat2FromBasis(Mat2 m, Vec2 a1, Vec2 a2) {}

void MathLib::initVec2FromPoints(Vec2 *v, Point2 a, Point2 b) {
	(*v).x = b.x - a.x;
	(*v).y = b.y - a.y;
}


float MathLib::Dot(Vec2 v1, Vec2 v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

void MathLib::AddPoint2Vec2(Point2 p1, Vec2 v1, Point2 *p2) {
	(*p2).x = p1.x + v1.x;
	(*p2).y = p1.y + v1.y;
}

void MathLib::AddVec2(Vec2 v1, Vec2 v2, Vec2 *v3) {
	(*v3).x = v1.x + v2.x;
	(*v3).y = v1.y + v2.y;
}

void MathLib::SubstractVec2(Vec2 v1, Vec2 v2, Vec2 *v3) {
	(*v3).x = v1.x - v2.x;
	(*v3).y = v1.y - v2.y;
}

void MathLib::scaleVec2(Vec2 v1, float n, Vec2 *v2) {
	(*v2).x = v1.x * n;
	(*v2).y = v1.y * n;
}

void MathLib::perpendicularVec2(Vec2 v1, Vec2 *v2) {
	(*v2).x = v1.y * -1;
	(*v2).y = v1.x;
}

float MathLib::LenghtVec2(Vec2 v1) {
	return sqrt(pow(v1.x, 2) + pow(v1.y, 2));
}

void MathLib::normalizeVec2(Vec2 v1, Vec2 *v2) {
	float length = LenghtVec2(v1);

	(*v2).x = v1.x / length;
	(*v2).y = v1.y / length;
}

void MathLib::projectionVec2(Vec2 v1, Vec2 v2, Vec2 *v3) {
	float factor = Dot(v1, v2) / pow(LenghtVec2(v2), 2);

	(*v3).x = v2.x * factor;
	(*v3).y = v2.y * factor;
}

void MathLib::combinePoint2(Point2 v1, Point2 v2, float f, Point2*) {

}

void MathLib::multMat2(Mat2 m1, Mat2 m2, Mat2 *m3) {
	(*m3)[0][0] = m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0];
	(*m3)[0][1] = m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1];
	(*m3)[1][0] = m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0];
	(*m3)[1][1] = m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1];
}

void MathLib::multMat2Vec2(Mat2 m1, Vec2 v1, Vec2 *v2) {
	(*v2).x = m1[0][0] * v1.x + m1[0][1] * v1.y;
	(*v2).y = m1[1][0] * v1.x + m1[1][1] * v1.y;
}


void MathLib::addMat2(Mat2 m1, Mat2 m2, Mat2 *m3) {
	int i, j;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			(*m3)[i][j] = m1[i][j] + m2[i][j];
		}
	}
}

void MathLib::scaleMat2(Mat2 m1, float n, Mat2 *m2) {
	int i, j;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			(*m2)[i][j] = m1[i][j] * n;
		}
	}
}

float MathLib::rads(float degrees) {
	float rads;
	rads = (degrees) / 180 * M_PI;
	return rads;
}

void MathLib::DrawVec2(Vec2 v, Point2 p) {
	Point2 a, b;
	Vec2 pp;
	float arrow_height = 20.0f;
	float pathPoints[10];

	a = p;
	initPoint2(&b, p.x + v.x, p.y + v.y);

	MOMOS::DrawSetStrokeColor(255, 255, 255, 255);
	MOMOS::DrawLine(a.x, a.y, b.x, b.y);

	perpendicularVec2(v, &pp);

}


/* Dibuja un cuadrado con su esquina superior izquierda en las coordenadas indicadas */
void MathLib::printSquare(Point2 p, Vec2 vectors[4], float color[]) {
	int i;
	float pathPoints[10];

	for (i = 0; i < 8; i += 2) {
		pathPoints[i] = p.x + vectors[i / 2].x;
		pathPoints[i + 1] = p.y + vectors[i / 2].y;
	}

	pathPoints[i] = p.x + vectors[0].x;
	pathPoints[i + 1] = p.y + vectors[0].y;

	/*Color rgb interior del pol�gono*/
	MOMOS::DrawSetStrokeColor(color[0], color[1], color[2], color[3]);

	/*Pinta la misma figura rellena. El �ltimo par�metro determina si se muestra el borde*/
	MOMOS::DrawPath(pathPoints, 5);
}


//Shortcut para la matrix de rotaci�n aplicada a un vector
void MathLib::rotateVec2(Vec2 *v, float angle) {
	float x, y;
	x = (*v).x;
	y = (*v).y;

	(*v).x = x*cos(angle) - y*sin(angle);
	(*v).y = x*sin(angle) + y*cos(angle);
}


void MathLib::afineTransformFromPoint2(Point2 current, Mat2 m, Point2 origin, Point2 *result) {
	Vec2 v1;

	initVec2FromPoints(&v1, current, origin);
	multMat2Vec2(m, v1, &v1);
	AddPoint2Vec2(origin, v1, &(*result));
}


void MathLib::drawLine(Point2 p1, Point2 p2) {
	MOMOS::DrawLine(p1.x, p1.y, p2.x, p2.y);
}

void MathLib::MultMat3(Mat3 m1, Mat3 m2, Mat3 *result) {}
void MathLib::MultMat3vec3(Mat3 m, Vec3 v, Vec3 *result) {}
void MathLib::Mat3Scale(Mat3 m1, float n, Mat3 *m2) {}
void MathLib::Mat3Rotate(Mat3 *m, float angle) {}
void MathLib::Mat3Translate() {}