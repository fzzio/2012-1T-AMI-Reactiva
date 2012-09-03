#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // Configuraciones iniciales
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    pantallaActual = PANTALLA_CONFIGURACION;

    /// Paneles
    panel.setup("Configuracion:", "configuraciones/configuraciones.xml", 10, 10);
    panel.add(umbralCerca.setup("Umbral Cerca", 230, 0, 250));
    panel.add(umbralLejos.setup("Umbral Lejos", 720, 0, 750));
    panel.add(lblAngulo.setup("lblangulo", ""));

    fuenteVagRounded.loadFont("fuentes/vag.ttf", 16);
    posicionInicialZ = 0;
    posActualZ = 0;
    distancia = 0;

    /// Acciones con el Kinect
	kinect.setRegistration(true); // enable depth->video image calibration

	kinect.init();
	// kinect.init(true); // shows infrared instead of RGB video image
	// kinect.init(false, false); // disable video image (faster fps)

	kinect.open();		// opens first available kinect

	kinectAngulo = 0;
	kinect.setCameraTiltAngle(kinectAngulo);
	bCapturarPosInicialZ = false;


    /// Imagenes
    imgFondoConfiguracion.loadImage("imagenes/linux-wall.png");
    imgFondoAplicacion.loadImage("imagenes/linuxpower.jpg");

    imgColor.allocate(kinect.width, kinect.height);
    imgGris.allocate(kinect.width, kinect.height);
    imgUmbralCercaGris.allocate(kinect.width, kinect.height);
    imgUmbralLejosGris.allocate(kinect.width, kinect.height);

}

//--------------------------------------------------------------
void testApp::update(){
    kinect.update();
    lblAngulo = "Angulo: " + ofToString(kinectAngulo);
    if(kinect.isFrameNew()) {
        imgColor.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);

        imgGris.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

        unsigned char * pix = imgGris.getPixels();
        int numPixels = imgGris.getWidth() * imgGris.getHeight();
        float* distanciaKinect = kinect.getDistancePixels(); // distancia en centimetros
        float posCercanaZ = 10000000000000000000.0f;
        for(int i = 0; i < numPixels; i++) {
            if(pix[i] < umbralCerca && pix[i] > umbralLejos) {
                pix[i] = 255;

                // verificamos cual es el mas proximo
                if(distanciaKinect[i] < posCercanaZ){
                    posCercanaZ = distanciaKinect[i];
                }
            } else {
                pix[i] = 0;
            }
        }

        posActualZ = posCercanaZ;

        // update the cv images
		imgGris.flagImageChanged();

		if(bCapturarPosInicialZ){

            posicionInicialZ = posActualZ;
            bCapturarPosInicialZ = false;
		}

        distancia = posActualZ - posicionInicialZ;


	}

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetupScreen();
    switch (pantallaActual){
        case PANTALLA_CONFIGURACION:
            dibujarPantallaConfiguracion();
            break;
        case PANTALLA_APLICACION:
            dibujarPantallaAplicacion();
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
		case '1':
			pantallaActual = PANTALLA_CONFIGURACION;
			break;

		case '2':
			pantallaActual = PANTALLA_APLICACION;
			break;

		case OF_KEY_UP:
			kinectAngulo++;
			if(kinectAngulo>30) kinectAngulo=30;
			kinect.setCameraTiltAngle(kinectAngulo);
			break;

		case OF_KEY_DOWN:
			kinectAngulo--;
			if(kinectAngulo<-30) kinectAngulo=-30;
			kinect.setCameraTiltAngle(kinectAngulo);
			break;

        case ' ':
            bCapturarPosInicialZ = true;
            break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::exit(){
    kinect.setCameraTiltAngle(0);
	kinect.close();

	// Liberamos imagenes
	imgColor.~ofxCvImage();
}


/// Metodos adicionales
void testApp::dibujarPantallaConfiguracion(){
    imgFondoConfiguracion.draw(floor(0.1), floor(0.1));
    panel.draw();

    //kinect.draw(10,10, 320, 240);
    kinect.draw(10, 350, 320, 240);
    kinect.drawDepth(350, 350, 320, 240);
    imgGris.draw(350, 10, 320, 240);

    char mensaje[255];
    sprintf(mensaje, "Posicion actual en Z: %0.2f", posActualZ);
    fuenteVagRounded.drawString(mensaje, 700, 30);
    sprintf(mensaje, "Distancia recorrida: %0.2f", distancia);
    fuenteVagRounded.drawString(mensaje, 700, 80);


}


void testApp::dibujarPantallaAplicacion(){
    imgFondoAplicacion.draw(floor(0.1), floor(0.1));
    imgColor.draw(10, 10, 480, 320);
}
