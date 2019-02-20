#ifndef CYLINDRE_H
#define CYLINDRE_H
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <math.h>
#include <GL/glu.h>
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QOpenGLBuffer>

class Cylindre
{
    public:
        float ep_cyl;
        float r_cyl;
        int nb_fac;
        float r,g,b;

        explicit Cylindre(float ep_cyl,float r_cyl,int nb_fac, float r, float g, float b);
        ~Cylindre();

        void dessiner();
        void dessiner_face(QMatrix4x4 * matrix, float ang);
        void dessiner_cote();
        int getSizeVert();
        QVector<GLfloat> vertData;
        QOpenGLBuffer return_vbo();
    private :
        QOpenGLShaderProgram *m_program;
        int  m_posAttr;
        int  m_colAttr;
        int m_matrixUniform;
        QOpenGLBuffer m_vbo;
        int sizeVertData;

};
#endif // CYLINDRE_H
