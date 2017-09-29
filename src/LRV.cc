#include "../include/LRV.h"

LRV::LRV() {
  pos_ = {400.0f, 200.0f};
  velocity_ = {0.25f, -1.0f};
  locked_ = true;
}


LRV::LRV(const LRV& orig) {
}


LRV::~LRV() {
}


void LRV::assignRectangle(std::vector<MathLib::Vec2>& buffer, int* counter) {
  *counter = 4;
  
  MathLib::Vec2 point;
          
  point = {-5.0f, 2.0f};
  buffer.insert (buffer.begin(), point);
  
  point = {-5.0f, -2.0f};
  buffer.insert (buffer.begin(), point);
  
  point = {5.0f, -2.0f}; 
  buffer.insert (buffer.begin(), point);
  
  point = {5.0f, 2.0f};
  buffer.insert (buffer.begin(), point);
}


void LRV::update() {
  move();
}


void LRV::move() {
  if (!locked_) {
    cpShapeSetFriction(physics_shape_, 0.5f);

    if (MOMOS::IsSpecialKeyPressed(MOMOS::kSpecialKey_Right)) {
      cpVect force = {0.03f, 0.0f};
      cpVect impulse_point = {-5.0f, 0.0f};
      cpBodyApplyForceAtLocalPoint(physics_body_, force , impulse_point);
    } else if (MOMOS::IsSpecialKeyPressed(MOMOS::kSpecialKey_Left)) {
      cpVect force = {-0.03f, 0.0f};
      cpVect impulse_point = {5.0f, 0.0f};
      cpBodyApplyForceAtLocalPoint(physics_body_, force , impulse_point);
    }

    //Update speed components
    velocity_.x = cpBodyGetVelocity(physics_body_).x;
    velocity_.y = cpBodyGetVelocity(physics_body_).y;

    //Do not allow the vehicle to roll upside down
    float rads = cpBodyGetAngle(physics_body_);
    float angle = rads * (57.2957);
    float threshold = 45.0f;
    if (angle > threshold) {
      cpBodySetAngle(physics_body_, 0.75f);
    } else if (angle < -threshold) {
      cpBodySetAngle(physics_body_, 0.75f);
    }
  }
}


void LRV::draw() {
  float rotation = cpBodyGetAngle(physics_body_);
  MOMOS::Mat3 translate, rotate, transform;
  MOMOS::Mat3InitAsTranslate(683.0f, pos_.y, &translate);
  MOMOS::Mat3InitAsRotate(rotation, &rotate);
  
  //Calculate transformed vertices
  float vertices_out[40];
  float vertex[2];
  float vertex_out[2];
 
  
  //Draw Quad
  for (int i=0; i<num_qvertices_; i++) {
    MOMOS::Mat3Multiply(translate, rotate, &transform);
	vertex[0] = qvertices_[i].x;
	vertex[1] = qvertices_[i].y;
	MOMOS::Mat3TransformVec2(transform, vertex, vertex_out);
	vertices_out[2*i] = vertex_out[0];
	vertices_out[2*i+1] = vertex_out[1];
  }
  MOMOS::DrawSetStrokeColor(255,255,255);
  MOMOS::DrawSolidPath(vertices_out, num_qvertices_, true);
}


void LRV::setPhysics() {
  
  physics_body_ = cpSpaceAddBody(space_, cpBodyNew(0,0));
  
  cpShape* sbox = cpSpaceAddShape(space_, cpBoxShapeNew(physics_body_, 12, 6, 5));
  physics_shape_ = sbox;
  
  cpPolyShape* poly =  cpPolyShapeAlloc();
  cpTransform transform = cpTransformNew(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  cpVect verts[500];
  
  //Create triangle
  for (int i=0; i<qvertices_.size(); i++) {
    verts[i] = cpVect {0.0f, 0.0f};
    verts[i].x = qvertices_[i].x;
    verts[i].y = qvertices_[i].y;
  }
  cpPolyShapeInit(poly, physics_body_, num_qvertices_, verts, transform, 1.0f);
  cpPolyShapeNew(physics_body_, num_qvertices_, verts, transform, 1.0f);
  
  cpShapeSetElasticity(sbox, 0);
  cpShapeSetFriction(sbox, 1.0f);
  cpShapeSetMass(sbox, 0.5);
  cpShapeSetCollisionType(sbox, LRV_TYPE);
  
  cpVect position = {pos_.x, pos_.y};
  cpBodySetPosition(physics_body_, position);
}