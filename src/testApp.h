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
        ofxButton               btnCapturar;

        void                    aprenderUmbralesPresionado(bool & presionado);

        // datos
        float                   posicionInicialZ;
        int                     posMinZ, posMaxZ;
        float                   posActualZ;
        float                   distancia;
        int                     distanciaPorcent;

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


        void dibujarPiso(int posX,int posY, int ancho, int alto, int porcentaje);
        void CargarFrases();
        void escogerFrase(int porcentaje);
        void dibujarAmbiente(int posX,int posY, int ancho, int alto, int porcentaje);

        int dist;
        string fraSelec;


        typedef struct{
                int     porcIni;
                int     porcFin;
                string  frase;
        }sFrases;


        sFrases     vFrases[5];
        ofImage     fondoApp;
        ofImage     imgRegla;

};
