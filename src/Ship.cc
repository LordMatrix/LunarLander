#include "../include/Ship.h"

Ship::Ship() {
  pos_ = {683.0f, 200.0f};
  velocity_ = {0.25f, -1.0f};
  fuel_ = 100.0f;
  landed_ = false;
  crashed_ = false;
  explosion_time_ = 0.0f;
}


Ship::Ship(const Ship& orig) {
}


Ship::~Ship() {
}


void Ship::assignRegularPolygon(int num_vertices, float radius, MathLib::Vec2 position_offset, float rotation_offset, std::vector<MathLib::Vec2>& buffer, int* counter) {
  float rad;
  float angle = 360/(num_vertices);
  
  *counter = num_vertices;
  
  for (int i = 0; i < num_vertices; i++) {
    rad = (angle*i + rotation_offset) / 180 * M_PI;
    MathLib::Vec2 point;
    point.x = cos(rad) * radius + position_offset.y;
    point.y = sin(rad) * radius + position_offset.y;
    buffer.insert (buffer.begin(), point);
  }
}


void Ship::explode() {
  explosion_time_++;
  int num_particles = 20;
  float x,y;
  float angle = MathLib::rads(360/num_particles);

  for (int i=0; i<num_particles; i++) {
    x = 683.0f + cos(angle*i) * explosion_time_*2;
    y = pos_.y + sin(angle*i) * explosion_time_*2;

    ESAT::DrawSetStrokeColor(255,255,255);          
    ESAT::DrawLine(x,y,x+2,y+2);
  }
  
  ESAT::DrawSetTextSize(40.0f);
  ESAT::DrawText(kWinWidth/4, kWinHeight/3, "The module has crashed");
  ESAT::DrawSetTextSize(20.0f);
  ESAT::DrawText(kWinWidth/4, kWinHeight/2.5, "Press space to retry");
}


void Ship::update() {
  if (!landed_) {
    move();
  } else {
    if (crashed_) {
      if (crashed_) {
        explode();
      } else {
        thrusting_ = false;
        explosion_time_ = 0.0f;
      }
      velocity_.x = cpBodyGetVelocity(physics_body_).x;
      velocity_.y = cpBodyGetVelocity(physics_body_).y;
    } else {
      thrusting_ = false;
    }
  }
}


void Ship::move() {
  thrusting_ = false;
  
  //Listen for launch
  if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Up) && fuel_>0.0f) {
    
    cpVect force = {0.0f, -0.02f};
    cpVect impulse_point = {0.0f, 5.0f};
    cpBodyApplyForceAtLocalPoint(physics_body_, force , impulse_point);
    fuel_ -= 0.1f;
    thrusting_ = true;
    cpShapeSetMass(this->physics_shape_, 0.5 + this->fuel_/100);
  }
  
  if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Right)) {
    cpBodySetAngle(physics_body_, cpBodyGetAngle(physics_body_) + 0.01f);
  } else if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Left)) {
    cpBodySetAngle(physics_body_, cpBodyGetAngle(physics_body_) - 0.01f);
  }
  
  //Update speed components
  velocity_.x = cpBodyGetVelocity(physics_body_).x;
  velocity_.y = cpBodyGetVelocity(physics_body_).y;
}


void Ship::drawThruster() {
  float rotation = cpBodyGetAngle(physics_body_);
  ESAT::Mat3 translate, rotate, transform;
  ESAT::Mat3InitAsTranslate(683.0f, pos_.y, &translate);
  ESAT::Mat3InitAsRotate(rotation, &rotate);
  
  float vertices[6] = {-5.0f,5.0f, 5.0f,10.0f, 0.0f,35.0f};
  //Calculate transformed vertices
  float vertices_out[40];
  float vertex[2];
  float vertex_out[2];
  
  //Draw circle
  for (int i=0; i<6; i+=2) {
        ESAT::Mat3Multiply(translate, rotate, &transform);
	vertex[0] = vertices[i];
	vertex[1] = vertices[i+1];
	ESAT::Mat3TransformVec2(transform, vertex, vertex_out);
	vertices_out[i] = vertex_out[0];
	vertices_out[i+1] = vertex_out[1];
  }
  ESAT::DrawSetStrokeColor(255,0,0);
  ESAT::DrawSolidPath(vertices_out, 3, true);
}


void Ship::draw() {
  float rotation = cpBodyGetAngle(physics_body_);
  ESAT::Mat3 translate, rotate, transform;
  ESAT::Mat3InitAsRotate(rotation+MathLib::rads(45), &rotate);
  
  if (!landed_ || crashed_) {
    ESAT::Mat3InitAsTranslate(683.0f, pos_.y, &translate);
  } else {
    ESAT::Mat3InitAsTranslate(pos_.x, pos_.y, &translate);
  }
  
  //Calculate transformed vertices
  float vertices_out[40];
  float vertex[2];
  float vertex_out[2];
  
  //Draw circle
  for (int i=0; i<num_cvertices_; i++) {
    ESAT::Mat3Multiply(translate, rotate, &transform);
    vertex[0] = cvertices_[i].x;
    vertex[1] = cvertices_[i].y;
    ESAT::Mat3TransformVec2(transform, vertex, vertex_out);
    vertices_out[2*i] = vertex_out[0];
    vertices_out[2*i+1] = vertex_out[1];
  }
  ESAT::DrawSetStrokeColor(255,255,255);
  ESAT::DrawSolidPath(vertices_out, num_cvertices_, true);
  
  
  //Draw triangle
  for (int i=0; i<num_tvertices_; i++) {
    ESAT::Mat3Multiply(translate, rotate, &transform);
    vertex[0] = tvertices_[i].x;
    vertex[1] = tvertices_[i].y;
    ESAT::Mat3TransformVec2(transform, vertex, vertex_out);
    vertices_out[2*i] = vertex_out[0];
    vertices_out[2*i+1] = vertex_out[1];
  }
  ESAT::DrawSetStrokeColor(255,255,255);
  ESAT::DrawSolidPath(vertices_out, num_tvertices_, true);
  
  if (thrusting_ && (int)ESAT::Time()%2==0) {
    drawThruster();
  }
  
  //Draw explosion
  if (crashed_) {
    explode();
  } else if (landed_) {
    ESAT::DrawSetTextSize(40.0f);
    ESAT::DrawText(kWinWidth/4, kWinHeight/3, "The module has landed");
    ESAT::DrawSetTextSize(20.0f);
    ESAT::DrawText(kWinWidth/4, kWinHeight/2.5, "Press space to fly again");
  }
}


void Ship::setPhysics() {
  physics_body_ = cpSpaceAddBody(space_, cpBodyNew(0,0));
  
  cpShape* sbox = cpSpaceAddShape(space_, cpBoxShapeNew(physics_body_, 20, 20, 0));
  physics_shape_ = sbox;
  
  cpPolyShape* poly =  cpPolyShapeAlloc();
  cpTransform transform = cpTransformNew(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  cpVect verts[500];
  
  //Create circle
  for (int i=0; i<cvertices_.size(); i++) {
    verts[i] = cpVect {0.0f, 0.0f};
    verts[i].x = cvertices_[i].x;
    verts[i].y = cvertices_[i].y;
  }
  cpPolyShapeInit(poly, physics_body_, num_cvertices_, verts, transform, 1.0f);
  
  
  //Create triangle
  for (int i=0; i<tvertices_.size(); i++) {
    verts[i] = cpVect {0.0f, 0.0f};
    verts[i].x = tvertices_[i].x;
    verts[i].y = tvertices_[i].y;
  }
  cpPolyShapeInit(poly, physics_body_, num_tvertices_, verts, transform, 1.0f);
  cpPolyShapeNew(physics_body_, num_tvertices_, verts, transform, 1.0f);
  
  cpShapeSetElasticity(sbox, 0);
  cpShapeSetFriction(sbox, 1.0f);
  cpShapeSetMass(sbox, 0.5);
  cpShapeSetCollisionType(sbox, SHIP_TYPE);
  
  cpVect position = {pos_.x, pos_.y};
  cpBodySetPosition(physics_body_, position);
}


void Ship::removePhysics() {
  cpSpaceRemoveBody(cpBodyGetSpace(physics_body_), physics_body_);
  cpSpaceRemoveShape(cpShapeGetSpace(physics_shape_), physics_shape_);

  cpShapeFree(physics_shape_);
}