#include "ClothSystem.h"


ClothSystem::ClothSystem(int w, int h)  : w(w), h(h) {
	m_numParticles = w*h;
	sweepMovement = false;
	wireMesh = false;
	ccw = true;
	angle = 0.f;
}

Vector3f ClothSystem::initPos() {
	return Vector3f::RIGHT + h*Vector3f::UP;
}

void ClothSystem::setFixedPoints() {
	PendulumSystem::setFixedPoints();
	fixedPoints.insert(w - 1);
}

void ClothSystem::addParticleInfo(int idx) {
	particleInfos.push_back(ParticleInfo(0.1f));
}

void ClothSystem::addSprings(int idx) {
	addStructuralSpring(idx);
	addFlexSpring(idx);
	addShearSpring(idx);
}

void ClothSystem::addStructuralSpring(int idx) {
	if (idx/w > 0) {
		genericAddSpring(idx - w, idx, 1.f, 7.f);
	}

	if (idx % w > 0) {
		genericAddSpring(idx - 1, idx, 1.f, 7.f);
	}
}

void ClothSystem::addShearSpring(int idx) {
	if ((idx % w > 0) && (idx/w > 0)) {
		// lower-right to upper-left
		genericAddSpring(idx - w - 1, idx, 1.414f, 7.f);
	}

	if ((idx % w < w - 1) && (idx/w > 0)) {
		// lower-left to upper-right
		genericAddSpring(idx - w + 1, idx, 1.414f, 7.f);
	}
}

void ClothSystem::addFlexSpring(int idx) {
	if (idx/w > 2) {
		genericAddSpring(idx - 2*w, idx, 2.f, 7.f, true);
	}

	if (idx % w > 2) {
		genericAddSpring(idx - 2, idx, 2.f, 7.f, true);
	}
}

void ClothSystem::updatePos(int idx, Vector3f& pos) {
	if ((idx % w) != (w - 1)) {
		pos += Vector3f::RIGHT; 
	} else {
		pos += (Vector3f::FORWARD - (w - 1)*Vector3f::RIGHT);
	}
}

void ClothSystem::updateVelocity(int idx, Vector3f& velocity) {
	return;
}

void ClothSystem::draw()
{
	if (wireMesh) {
    	PendulumSystem::draw();
		drawLines();
	} else {
		drawTriangles();
	}
}

void ClothSystem::drawLines() {
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glDisable( GL_LIGHTING ); 
	glColor4f( 1, 1, 1, 1 );
	glLineWidth( 1 );
	glBegin( GL_LINES);
	for (auto spring: springCollection) {
		if (spring.isFlex) continue;
		int p1 = spring.p1;
		int p2 = spring.p2;

		Vector3f pos1 = getState()[2*p1];
		Vector3f pos2 = getState()[2*p2];
		glVertex3f(pos1[0], pos1[1], pos1[2]);
		glVertex3f(pos2[0], pos2[1], pos2[2]);
	}
	glEnd();
	glPopAttrib();
}

void ClothSystem::drawTriangles() {
	vector<Vector3f> st = getState();
	vector<Vector3f> normals((w - 1)*(h - 1), Vector3f::ZERO);

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);
	for (int r = 0; r < h - 1; r++) {
		for (int c = 0; c < w - 1; c++) {  // Draw per quad
			// Indices
			int topLeft = r*w + c;
			int topRight = topLeft + 1;
			int botLeft = (r + 1)*w + c;
			int botRight = botLeft + 1;

			// Normals
			Vector3f nTopLeft = calcNormal(r, c, st, normals);
			Vector3f nTopRight = calcNormal(r, c + 1, st, normals);
			Vector3f nBotLeft = calcNormal(r + 1, c, st, normals);
			Vector3f nBotRight = calcNormal(r + 1, c + 1, st, normals);

			// Vertices
			Vector3f pTopLeft = st[2*topLeft];
			Vector3f pTopRight = st[2*topRight];
			Vector3f pBotLeft = st[2*botLeft];
			Vector3f pBotRight = st[2*botRight];

			glNormal3fv(nTopLeft);
			glVertex3fv(pTopLeft);

			glNormal3fv(nBotRight);
			glVertex3fv(pBotRight);

			glNormal3fv(nTopRight);
			glVertex3fv(pTopRight);

			glNormal3fv(nTopLeft);
			glVertex3fv(pTopLeft);

			glNormal3fv(nBotLeft);
			glVertex3fv(pBotLeft);

			glNormal3fv(nBotRight);			
			glVertex3fv(pBotRight);
		}
	}
	glEnd();
	glPopAttrib();
}

Vector3f ClothSystem::calcNormal(int r, int c, const vector<Vector3f>& st, vector<Vector3f>& normals) {
	Vector3f n;
	Vector3f p = st[2*(r*w + c)];

	bool hasLeft = c > 0; // cached
	bool hasRight = c < w - 1;  // calculated (then cached)

	bool hasTop = r > 0;
	bool hasBot = r < h - 1;

	if (hasRight) {
		Vector3f right = st[2*(r*w + c + 1)];
		if (hasTop) {
			Vector3f top = st[2*((r - 1)*w + c)];
			Vector3f northEast = Vector3f::cross(right - p, top - p).normalized();
			n += northEast;
			normals[(r - 1)*(w - 1) + c] = northEast;
		}
		if (hasBot) {
			Vector3f bot = st[2*((r + 1)*w + c)];
			Vector3f southEast = Vector3f::cross(bot - p, right - p).normalized();
			n += southEast;
			normals[r*(w - 1) + c] = southEast;
		}
	}

	if (hasLeft) {
		if (hasTop) {
			n += normals[(r - 1)*(w - 1) + (c - 1)];
		}
		if (hasBot) {
			n += normals[r*(w - 1) + (c - 1)];
		}
	}

	return n.normalized();
}

void ClothSystem::sweepFixedPoints(float h) {
	vector<Vector3f> st = getState();
	int f1 = 0;
	int f2 = w - 1;

	Vector3f p1 = st[2*f1];
	Vector3f p2 = st[2*f2];

	Vector3f m = (p1 + p2)/2.f;

	Matrix4f tx = Matrix4f::translation(-Vector3f(m));
	Matrix4f ntx = Matrix4f::translation(Vector3f(m));

	float a = M_PI*h/5.f;
	if (angle > M_PI) {
		ccw = !ccw;
		angle = a;
	} else {
		angle += a;
	}
		
	Matrix4f t1 = ntx*Matrix4f::rotateY(ccw? a : -a)*tx;

	st[2*f1] = (t1*Vector4f(p1, 1.f)).xyz();
	st[2*f2] = (t1*Vector4f(p2, 1.f)).xyz();

	setState(st);
}