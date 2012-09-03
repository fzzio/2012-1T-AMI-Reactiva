#pragma once

#include "ofMain.h"

// Incluimos los addons
    #include "ofxGui.h"
    #include "ofxOpenCv.h"
    #include "ofxKinect.h"

// Los datos y constantes a usar
    #define PANTALLA_CONFIGURACION  0
    #define PANTALLA_APLICACION     1

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // Para los controles
        ofxPanel                panel;
        ofxIntSlider            umbralLejos, umbralCerca;
        ofxToggle               cercaBlanco, umbral;
        ofxLabel                lblAngulo;

        // datos
        float                   posicionInicialZ;
        float                   posActualZ;
        float                   distancia;

        // Variables para usar kinect
        ofxKinect               kinect;
        int                     kinectAngulo;
        bool                    bCapturarPosInicialZ;
        // Imagenes de OpenCV
        ofxCvColorImage imgColor;
        ofxCvGrayscaleImage imgGris;
        ofxCvGrayscaleImage imgUmbralCercaGris;
        ofxCvGrayscaleImage imgUmbralLejosGris;

        // Variables agregadas
		int                     pantallaActual;
        ofImage                 imgFondoConfiguracion, imgFondoAplicacion;
        ofTrueTypeFont 	        fuenteVagRounded;

		// Metodos
		void                    dibujarPantallaConfiguracion();
		void                    dibujarPantallaAplicacion();



};
