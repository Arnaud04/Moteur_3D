#include "cylindre.h"
#include <math.h>
#include <GL/glu.h>
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QtMath>
#define M_PI 3.14159265358979323846

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";

Cylindre::Cylindre(float ep_cyl, float r_cyl, int nb_fac, float r, float g, float b)
{
    qDebug()<<"constructeur de cylindre";
    this->sizeVertData = 0;
    this->ep_cyl = ep_cyl;
    this->r_cyl = r_cyl;
    this->nb_fac = nb_fac;
    //m_vbo = new QOpenGLBuffer();
    //m_vbo.create();
    //dessiner();

}
void Cylindre::dessiner_face(QMatrix4x4 * matrix, float ang)
{
    qDebug()<<"Cylindre::dessiner_face";
    float angle = 2* M_PI/this->nb_fac;

    /*glBegin(GL_LINE_LOOP);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(this->r_cyl,0.0, 0.0);
    glVertex3f(cos(angle) * this->r_cyl, sin(angle)  *this->r_cyl, 0.0);
    glEnd();*/


    GLfloat vertices[] = {
    0.0,0.0,0.0,
    this->r_cyl,0.0,0.0,
    static_cast<GLfloat>(cos(angle)*this->r_cyl), static_cast<GLfloat>(sin(angle)*this->r_cyl), 0.0
    };

    GLfloat colors[] = {
        1.0, 0.0f, 0.0f,
        1.0, 0.0f, 0.0f,
        1.0, 0.0f, 0.0f
    };

    float radius = this->r_cyl;
    int nb_face = this->nb_fac;
    float z1 = -this->ep_cyl/2;
    float z2 = this->ep_cyl/2;
    //float alpha = 2* M_PI/this->nb_fac;
    float alpha = qDegreesToRadians(360.0/nb_face);
    QColor color = QColor(0,250,0,250);

    GLfloat vertices1[nb_face*3];    // Tableau des coordonnées des cercles
    GLfloat colors1[nb_face*3];      // Tableau des couleurs pour les cercles

    GLfloat vertices2[4*3];     // Tableau des coordonnées des facettes
    GLfloat colors2[4*3];       // Tableau des couleurs pour les facettes

    // Remplissage des coordonnées et des couleurs du premier cercle
    for (int i = 0; i < nb_face; ++i) {
        vertices1[i*3] = radius*qCos(i*alpha);
        colors1[i*3] = color.redF();
        vertices1[i*3 + 1] = radius*qSin(i*alpha);
        colors1[i*3 + 1] = color.greenF();
        vertices1[i*3 + 2] = z1;
        colors1[i*3 + 2] = color.blueF();
    }

    //glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    //glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    /*glEnableVertexAttribArray(m_posAttr);  // rend le VAA accessible pour glDrawArrays
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_LINE_LOOP, 0, 6);

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);*/

    //QVector<GLfloat> vertData;
    //matrix->rotate(ang, 0, 1, 0);

    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData.append(vertices1[i*3+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++)
            vertData.append(colors1[i*3+j]);
    }

    //m_vbo->allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    //glDrawArrays(GL_TRIANGLES, 0, vertData.size());


    //m_vbo.bind();
    //m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    this->sizeVertData = this->sizeVertData + vertData.size();
}
void Cylindre::dessiner_cote()
{
    qDebug()<<"cylindre::dessiner_cote";
    float angle = 2* M_PI/this->nb_fac;
    /*glBegin(GL_LINE_LOOP);
    glVertex3f(this->r_cyl,0.0, this->ep_cyl/2);
    glVertex3f(this->r_cyl,0.0, -this->ep_cyl/2);
    glVertex3f(cos(angle) * this->r_cyl, sin(angle)  *this->r_cyl, -this->ep_cyl/2);
    glVertex3f(cos(angle) * this->r_cyl, sin(angle)  *this->r_cyl, this->ep_cyl/2);
    glEnd();*/

    GLfloat vertices[] = {
    this->r_cyl,0.0, this->ep_cyl/2,
    this->r_cyl,0.0, -this->ep_cyl/2,
    static_cast<GLfloat>(cos(angle) * this->r_cyl), static_cast<GLfloat>(sin(angle)  *this->r_cyl), static_cast<GLfloat>(-this->ep_cyl/2),
    static_cast<GLfloat>(cos(angle) * this->r_cyl), static_cast<GLfloat>(sin(angle)  *this->r_cyl), static_cast<GLfloat>(this->ep_cyl/2)
    };

    GLfloat colors[] = {
        1.0, 0.0f, 0.0f,
        1.0, 0.0f, 0.0f,
        1.0, 0.0f, 0.0f,
        0.0, 1.0, 0.0
    };
    //QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData.append(vertices[i*3+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++)
            vertData.append(colors[i*3+j]);
    }
    //glDrawArrays(GL_TRIANGLES, 0, vertData.size());

    //m_vbo.bind();
    //m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    this->sizeVertData = this->sizeVertData + vertData.size();

}

int Cylindre::getSizeVert()
{
    qDebug()<<"size Vert DATA : "<<this->sizeVertData;
    return this->sizeVertData;
}

QOpenGLBuffer Cylindre::return_vbo()
{
    qDebug()<<"retour du VBO de cylindre";

    return m_vbo;
}

void Cylindre::dessiner()
{
    qDebug()<<"appel de cylindre::dessiner";
    QMatrix4x4 matrix;
    //matrix.translate(0, 0, 3);
    for (int i= 0; i < this->nb_fac; i++)
    {
        //QMatrix4x4 matrix;

        float angle = qDegreesToRadians(360.0f * i / this->nb_fac);
        //glColor3f (this->r, this->g, this->b);
        dessiner_face(&matrix, angle);
        //matrix.rotate(angle, 0, 1, 0);


    }

   /* matrix.translate(0, 0, -this->ep_cyl);
    for (int i= 0; i < this->nb_fac; i++)
    {
        //QMatrix4x4 matrix;
        float angle = (360.0f * i / this->nb_fac);


        matrix.rotate(angle, 0, 1, 0);
        //glTranslated(0,0,-this->ep_cyl/2);
        //glVertex3f(cos(angle/i)*this->r_cyl, sin(angle/i) *this->r_cyl, this->ep_cyl/2);


        //glColor3f (this->r, this->g, this->b);
        dessiner_face();


    }
    //matrix.translate(0, 0, -this->ep_cyl/2);
    for (int i= 0; i < this->nb_fac; i++)
    {

        float angle = (360.0f * i / this->nb_fac);
        //QMatrix4x4 matrix;

        matrix.rotate(angle, 0, 1, 0);

        //glVertex3f(cos(angle/i)*this->r_cyl, sin(angle/i) *this->r_cyl, this->ep_cyl/2);
        dessiner_cote();

    }*/

}

