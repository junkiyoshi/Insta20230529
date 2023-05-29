#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SUBTRACT);

	auto ico_sphere = ofIcoSpherePrimitive(130, 4);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	ofSeedRandom(39);

	int hue_start = 0;
	for (int x = -150; x <= 150; x += 300) {

		for (int y = -150; y <= 150; y += 300) {

			auto noise_seed = ofRandom(1000);
			hue_start += 64;
			for (int k = 0; k < this->triangle_list.size(); k++) {

				glm::vec3 avg = (this->triangle_list[k].getVertex(0) + this->triangle_list[k].getVertex(1) + this->triangle_list[k].getVertex(2)) / 3;

				vector<glm::vec3> vertices;
				vertices.push_back(this->triangle_list[k].getVertex(0));
				vertices.push_back(this->triangle_list[k].getVertex(1));
				vertices.push_back(this->triangle_list[k].getVertex(2));

				for (auto& v : vertices) {

					auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(v * 0.005, noise_seed + ofGetFrameNum() * 0.0085)), 0, 1, -220, 220) * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
					auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(v * 0.005, noise_seed + ofGetFrameNum() * 0.0085)), 0, 1, -220, 220) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
					auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec4(v * 0.005, noise_seed + ofGetFrameNum() * 0.0085)), 0, 1, -220, 220) * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));
					v = glm::vec4(v, 0) * rotation_z * rotation_y * rotation_x;

					v += glm::vec3(x, y, 0);
				}

				this->mesh.addVertices(vertices);

				ofColor color;
				color.setHsb((int)ofMap(avg.z, -130, 130, hue_start, hue_start + 128) % 255, 255, 255);
				for (int i = 0; i < 3; i++) {

					this->mesh.addColor(ofColor(color, 16));
				}

				this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	//ofRotateX(ofGetFrameNum());
	//ofRotateY(ofGetFrameNum());

	this->mesh.draw();

	this->cam.end();
}


//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}