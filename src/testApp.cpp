#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // Configuraciones iniciales
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    pantallaActual = PANTALLA_CONFIGURACION;

    /// Paneles
    panel.setup("Configuracion:", "configuraciones/panel.xml", 10, 10);
    panel.add(umbralCerca.setup("Umbral Cercano", 230, 0, 1000));
    panel.add(umbralLejos.setup("Umbral Lejano", 72, 0, 1000));
    panel.add(lblAngulo.setup("lblangulo", ""));
    panel.add(btnCapturar.setup("Guardar configuracion"));

    fuenteVagRounded.loadFont("fuentes/vag.ttf", 16);
    CargarFrases();

    fondoApp.loadImage("imagenes/fondo.png");
    imgRegla.loadImage("imagenes/regla.png");

    btnCapturar.addListener(this,&testApp::aprenderUmbralesPresionado);

    posicionInicialZ = 0;
    posMinZ = umbralCerca;
    posMaxZ = umbralLejos;
    posActualZ = 0;
    distancia = 0;
    distanciaPorcent = 0;

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

    panel.loadFromFile("configuraciones/panel.xml");

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

                if(distanciaKinect[i] < posCercanaZ){
                    posCercanaZ = distanciaKinect[i];
                }

            } else {
                pix[i] = 0;
            }
        }

        posActualZ = posCercanaZ;

        if(bCapturarPosInicialZ){
            posicionInicialZ = posActualZ;
            bCapturarPosInicialZ = false;
		}

        imgGris.flagImageChanged();

        distancia = fabs(posActualZ - posicionInicialZ);

        distanciaPorcent = (distancia * 100 )/ fabs(umbralLejos - umbralCerca);
        if (distanciaPorcent < 0){
            distanciaPorcent = 0;
        }else if(distanciaPorcent > 100){
            distanciaPorcent = 100;
        }
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

    btnCapturar.removeListener(this,&testApp::aprenderUmbralesPresionado);
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
    sprintf(mensaje, "Distancia porcentaje: %d", distanciaPorcent);
    fuenteVagRounded.drawString(mensaje, 700, 130);


}


void testApp::dibujarPantallaAplicacion(){

    dibujarAmbiente(747,15,200,733, distanciaPorcent);

    imgColor.draw(116, 125, 480, 320);
}

//Dibujo piso

void testApp::dibujarAmbiente(int posX,int posY, int ancho, int alto, int porcentaje){
    fondoApp.draw(floor(0.1), floor(0.1));
    imgRegla.draw(668, 15);
    dibujarPiso(posX,posY, ancho, alto, porcentaje);

}
void testApp:: dibujarPiso(int posX,int posY, int ancho, int alto, int porcentaje){

    int drec;int pos;


    drec = (alto*porcentaje)/100;
    pos = (posY+alto) - drec;


    escogerFrase(porcentaje); //Escogemos frase dependiendo del porcentaje

    ofPushStyle();

    if (porcentaje >= 0 && porcentaje <= 20){

        ofSetColor(255,0,0);
    }

    if (porcentaje > 20 && porcentaje <= 40){
        ofSetColor(255,1,255);
    }

     if (porcentaje > 40 && porcentaje <= 60){

       ofSetColor(0,0,255);
    }

     if (porcentaje > 60 && porcentaje <= 80){
        ofSetColor(255,255,0);
    }

     if (porcentaje > 80 && porcentaje <= 100){
        ofSetColor(0,255,0,200);
    }

    ofRect(posX,pos,ancho,drec);
    ofSetColor(0,0,0);
    fuenteVagRounded.drawString(fraSelec, posX + 20, pos + 20);
    ofPopStyle();

}

void testApp:: escogerFrase(int porcentaje){
 if (porcentaje >= 0 && porcentaje <= 20){
        fraSelec =  vFrases[0].frase;

    }

    if (porcentaje >= 20 && porcentaje <= 40){
       fraSelec =  vFrases[1].frase;
    }

     if (porcentaje >= 40 && porcentaje <= 60){
        fraSelec =  vFrases[2].frase;
    }

     if (porcentaje >= 60 && porcentaje <= 80){
        fraSelec =  vFrases[3].frase;
    }

     if (porcentaje >= 80 && porcentaje <= 100){
        fraSelec =  vFrases[3].frase;
    }



}
//Dibujo piso
void testApp:: CargarFrases(){

    vector <string> seussLines;
    int i;
    int porIni, porFin;
    i = 0;

    ofBuffer buffer = ofBufferFromFile("frases/frases.txt");

    if(buffer.size()) {
        while(buffer.isLastLine() == false) {
            string line = buffer.getNextLine();

            if(line.empty() == false) {
                seussLines = ofSplitString(line,"|");

                vFrases[i].porcIni = ofToInt(seussLines[0]);
                vFrases[i].porcFin = ofToInt(seussLines[1]);
                vFrases[i].frase= ofToString(seussLines[2]);
                i++;
                seussLines.clear();
            }

            }
    }
}

void testApp::aprenderUmbralesPresionado(bool & presionado){
    //todos los fondos
    if(presionado){
        panel.saveToFile("configuraciones/panel.xml");
    }
}
