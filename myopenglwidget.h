#ifndef MYOPENGLWIDGET
#define MYOPENGLWIDGET

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "mymesh.h"
#include "lightsourceobject.h"
#include <vector>
#include <list>
#include <gl/gl.h>

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QOpenGLTexture>
#define PI 3.14159


const float twoPi = 2.0f * PI;
const float Radius = 0.1;

using namespace std;
using namespace OpenMesh;

struct Coordinate
{
   float x;
   float y;
};



class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    TriMesh* mesh;
    MyOpenGLWidget(QWidget* parent = 0)
        :QOpenGLWidget(parent),
         vbo(QOpenGLBuffer::VertexBuffer),
         ibo(QOpenGLBuffer::IndexBuffer),
         cbo(QOpenGLBuffer::VertexBuffer),
         program(),
         indexSize(0)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    void draw()
    {
        repaint();
    }

    void setRenderMode(int m)
    {
        render_mode = m;
        repaint();
    }
    void setDeformMode(int m){
        deform_mode = m;
    }
    void setTopView()
    {
        deg_x = 0;
        deg_y = 90;
        //step = -3;
        e_x = 0;
        e_y = 0;
        repaint();
    }
    void setBottomView()
    {
        deg_x = 0;
        deg_y = -90;
        //step = -3;
        e_x = 0;
        e_y = 0;
        repaint();
    }
    void setLeftView()
    {
        deg_x = -90;
        deg_y = 0;
        //step = -3;
        e_x = 0;
        e_y = 0;
        repaint();
    }
    void setRightView()
    {
        deg_x = 90;
        deg_y = 0;
        //step = -3;
        e_x = 0;
        e_y = 0;
        repaint();
    }
    void setFrontView()
    {
        deg_x = 180;
        deg_y = 0;
        //step = -3;
        e_x = 0;
        e_y = 0;
        repaint();
    }
    void setBackView()
    {
        deg_x = 0;
        deg_y = 0;
        //step = -3;
        e_x = 0;
        e_y = 0;
        repaint();
    }

    void UpdateMesh() {
        // This method is invoked in UI thread!!
        // Need to makeCurrent!!!!
        makeCurrent();
        mesh->update_normals();
        vertexData.clear();
        colorData.clear();
        inv_colorData.clear();
        normalData.clear();
        indexData.clear();
        vector<int> sel_ind;
        sel_ind.clear();

        // render selected faces
        for(unsigned int i = 0;i < mesh->select_mfh.size(); i++)
        {
            int j = 0;
            for(TriMesh::FVIter fv_it= mesh->fv_iter(mesh->select_mfh[i]); fv_it ; ++fv_it, j = (j + 1) % 3){
                sel_ind.push_back((int)fv_it->idx());
            }
        }
        sort(sel_ind.begin(),sel_ind.end());
//        for(int i = 0;i<sel_ind.size();i++)
//            cout<<sel_ind[i]<<" ";
//        cout<<endl;
        int ind_cnt = 0;
       /* for(TriMesh::FaceIter f_it = mesh->faces_begin();f_it!=mesh->faces_end();++f_it)
        {
            for(TriMesh::FVIter fv_it= mesh->fv_iter(f_it.handle()); fv_it ; ++fv_it){
                TriMesh::Point p = mesh->point(*fv_it);
                vertexData.push_back(p[0]);
                vertexData.push_back(p[1]);
                vertexData.push_back(p[2]);
               // TriMesh::Color c = mesh->color(*fv_it);
                vector<int>::iterator iter=std::find(sel_ind.begin(),sel_ind.end(),(int)fv_it->idx());
                if(iter==sel_ind.end()){
                    colorData.push_back(100.0f/255.0f);
                    colorData.push_back(147.0f/255.0f);
                    colorData.push_back(237.0f/255.0f);
                    inv_colorData.push_back(1.0f-100.0f/255.0f);
                    inv_colorData.push_back(1.0f-147.0f/255.0f);
                    inv_colorData.push_back(1.0f-237.0f/255.0f);
                }
                else{
                    colorData.push_back(255.0f/255.0f);
                    colorData.push_back(182.0f/255.0f);
                    colorData.push_back(193.0f/255.0f);
                    inv_colorData.push_back(1.0f-255.0f/255.0f);
                    inv_colorData.push_back(1.0f-182.0f/255.0f);
                    inv_colorData.push_back(1.0f-193.0f/255.0f);
                }
                TriMesh::Normal n = mesh->normal(*fv_it);
                normalData.push_back(n[0]);
                normalData.push_back(n[1]);
                normalData.push_back(n[2]);
                ind_cnt++;
            }

        }
        */
        int cnt = 0;
        for(TriMesh::VIter v_it = mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it) {
            TriMesh::Point p = mesh->point(*v_it);
            vertexData.push_back(p[0]);
            vertexData.push_back(p[1]);
            vertexData.push_back(p[2]);
            TriMesh::Color c = mesh->color(*v_it);
            vector<int>::iterator iter=std::find(sel_ind.begin(),sel_ind.end(),cnt);
            if(iter==sel_ind.end()){
                colorData.push_back(100.0f/255.0f);
                colorData.push_back(147.0f/255.0f);
                colorData.push_back(237.0f/255.0f);
                inv_colorData.push_back(1.0f-100.0f/255.0f);
                inv_colorData.push_back(1.0f-147.0f/255.0f);
                inv_colorData.push_back(1.0f-237.0f/255.0f);
            }
            else{
                colorData.push_back(255.0f/255.0f);
                colorData.push_back(182.0f/255.0f);
                colorData.push_back(193.0f/255.0f);
                inv_colorData.push_back(1.0f-255.0f/255.0f);
                inv_colorData.push_back(1.0f-182.0f/255.0f);
                inv_colorData.push_back(1.0f-193.0f/255.0f);
                //cout<<"color!!"<<endl;
            }
            TriMesh::Normal n = mesh->normal(*v_it);
            normalData.push_back(n[0]);
            normalData.push_back(n[1]);
            normalData.push_back(n[2]);
            cnt++;
        }
        for(TriMesh::FIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it) {
            for(TriMesh::FVIter fv_it = mesh->fv_iter( *f_it ); fv_it; ++fv_it) {
                indexData.push_back((unsigned int)fv_it->idx());
            }
        }


//        vbo.bind();
//        vbo.allocate(vertexData.data(), vertexData.size() * sizeof(float));
//        program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
//        cbo.bind();
//        cbo.allocate(colorData.data(), colorData.size() * sizeof(float));
//        program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

//        nbo.bind();
//        nbo.allocate(normalData.data(), normalData.size() * sizeof(float));
//        program.setAttributeBuffer(2, GL_FLOAT, 0, 3);
        ibo.bind();
        ibo.allocate(indexData.data(), indexData.size() * sizeof(unsigned int));
        indexSize = indexData.size();
//        qDebug() << vertexData.size();
//        qDebug() << indexData.size();
//        step = -5;
//        deg_x = 0;
//        deg_y = 0;
//        e_x = 0;
//        e_y = 0;
//        eye = QVector3D(0,0,step);
//        center = QVector3D(0,0,0);
//        up = QVector3D(0,1,0);
        repaint();
    }
    void Find_Silhouette()
    {
        makeCurrent();
        mesh->Find_Silhouette(0,step*-sin(deg_y),step*cos(deg_y),deg_x);
        silhouette_flag = 1;
        repaint();
    }
protected:
    void initializeGL()
    {
        // initialize opengl here

        count = 1;
        dx = 0;
        dy = 0;
        x_y = 92.2233;
        x_y_in = 1 / x_y;
        centerx = 263;
        centery = 382;
        //faceColors[0] = Qt::red;
        //faceColors[1] = Qt::green;
        //faceColors[2] = Qt::blue;
        //faceColors[3] = Qt::yellow;

         qDebug() << "INIT GL"<<endl;
        initializeOpenGLFunctions();
        glClearColor(0.2, 0.2, 0.2, 1);
        glEnable(GL_DEPTH_TEST);
        vbo.create();
        ibo.create();
        cbo.create();
        nbo.create();
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":basic.vs.glsl");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":basic.fs.glsl");
        program.bindAttributeLocation("vertex", 0);
        program.bindAttributeLocation("color", 1);
        program.bindAttributeLocation("av3normal",2);
        program.link();
        program.bind();
        qDebug() << program.log();
        deg_x = 0;
        deg_y = 0;
        step = -5;
        e_x = 0;
        e_y = 0;
        eye = QVector3D(0,0,step);
        center = QVector3D(0,0,0);
        up = QVector3D(0,1,0);
        shift_flag = 0;
        silhouette_flag = 0;
        render_mode = 0;
        deform_mode = 0;
        //mesh->select_mfh.clear();
        setLightSource();
        QVector4D m_a = QVector4D(0.25,0.25,0.25,1.0);
        QVector4D m_d = QVector4D(0.4,0.4,0.4,1.0);
        QVector4D m_s = QVector4D(0.7746,0.7746,0.7716,1.0);
        float s = 0.6;
        click_point = QVector3D(0.0,0.0,0.0);
        program.setUniformValue("Material.ambient",m_a);
        program.setUniformValue("Material.diffuse",m_d);
        program.setUniformValue("Material.specular",m_s);
        program.setUniformValue("Material.shininess",s);

    }
    void setLightSource()
    {
        dir_light =  new LightSourceObject();
        QVector4D p = QVector4D(dir_light->m_Position[0],dir_light->m_Position[1],dir_light->m_Position[2],1.0f);
        QVector4D a = QVector4D(dir_light->ambient[0],dir_light->ambient[1],dir_light->ambient[2],1.0f);
        QVector4D d = QVector4D(dir_light->diffuse[0],dir_light->diffuse[1],dir_light->diffuse[2],1.0f);
        QVector4D s = QVector4D(dir_light->specular[0],dir_light->specular[1],dir_light->specular[2],1.0f);
        program.setUniformValue("Light.position", p);
        program.setUniformValue("Light.ambient", a);
        program.setUniformValue("Light.diffuse", d);
        program.setUniformValue("Light.specular", s);
        float m_CA = 1.0f;
        float m_LA = 4.5f/(float)dir_light->Point_Light_Range;
        float m_QA = 75.0f/((float)dir_light->Point_Light_Range*(float)dir_light->Point_Light_Range);
        program.setUniformValue("Light.constantAttenuation", m_CA);
        program.setUniformValue("Light.linearAttenuation", m_LA);
        program.setUniformValue("Light.quadraticAttenuation", m_QA);

        program.setUniformValue("Light.spotCosCutoff", dir_light->m_spotCosCutoff);
        program.setUniformValue("Light.spotCutoff", dir_light->m_spotCutoff);
        program.setUniformValue("Light.spotExponent", dir_light->m_spotExponent);

    }
    void unProject(double x,double y,double z, QMatrix4x4 M,QMatrix4x4 P, GLint V[4],double *objx,double *objy,double *objz){
        QMatrix4x4 finalMatrix;
        double in[4]={x,y,z,1.0f};
        finalMatrix = P * M;
        finalMatrix = finalMatrix.inverted();
        in[0] = ((in[0]-(double)V[0])/(double)V[2])*2.0f-1.0f;
        in[1] = ((in[1]-(double)V[1])/(double)V[3])*2.0f-1.0f;
        in[2] = in[2]*2.0f - 1.0f;
        QVector4D tem = QVector4D(in[0],in[1],in[2],1.0f);
        tem = finalMatrix * tem;
        if(tem.w()==0) cout<< "multiplication error"<<endl;
        tem.setX(tem.x()/tem.w());
        tem.setY(tem.y()/tem.w());
        tem.setZ(tem.z()/tem.w());
        *objx = tem.x();
        *objy = tem.y();
        *objz = tem.z();

        cout<<"obj coordinate: "<<*objx<<" "<<*objy<<" "<<*objz<<endl;

    }


    void del_Face(int x,int y){
        makeCurrent();
        GLint V[4];
        glGetIntegerv(GL_VIEWPORT,V);
        float depth_buffer;
        glReadPixels( (int) x, (int) V[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,  &depth_buffer );
        //cout<< depth_buffer<<endl;

        //cout<<"View Port: "<<V[0]<<" "<<V[1]<<" "<<V[2]<<" "<<V[3]<<endl;
        QMatrix4x4 M,P;
        M.setToIdentity();
        P.setToIdentity();
        eye = QVector3D(0+e_x,step*-sin(deg_y)+e_y,step*cos(deg_y));
        center = QVector3D(e_x,e_y,0);
        up = QVector3D(0,cos(deg_y),0);
        M.lookAt(eye,center,up);
        M.rotate(deg_x,0,1,0);
        qreal zNear = 0.1, zFar = 10.0, fov = 45.0;
        P.perspective(fov, aspect, zNear, zFar);
        double objx,objy,objz;
        //cout<<x<<" "<<V[3]-y<<endl;
        unProject(x,V[3]-y,depth_buffer,M,P,V,&objx,&objy,&objz);

        //TriMesh::Point click_p(objx,objy,objz);
        QVector3D click_p = QVector3D(objx,objy,objz);
        double error = 1.0f;
        select_fh = TriMesh::InvalidFaceHandle;
        for(TriMesh::FIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it)
        {
            //TriMesh::Point p[3];
            QVector3D p[3];
            int i = 0;
            for(TriMesh::FVIter fv_it = mesh->fv_iter( *f_it ); fv_it; ++fv_it) {
                TriMesh::Point tem_p = mesh->point(fv_it);
                p[i++] = QVector3D(tem_p[0],tem_p[1],tem_p[2]);
            }
//            cout<<"p1"<<p[0].x()<<" "<<p[0].y()<<" "<<p[0].z()<<endl;
//            cout<<"p2"<<p[1].x()<<" "<<p[1].y()<<" "<<p[1].z()<<endl;
//            cout<<"p3"<<p[2].x()<<" "<<p[2].y()<<" "<<p[2].z()<<endl;
            //TriMesh::Point ab = p[1] - p[0], bc = p[2] - p[1], ca = p[0] - p[2];
            QVector3D ab = p[1] - p[0], bc = p[2] - p[1], ca = p[0] - p[2];
//            cout<<"ab: "<<ab[0]<<" "<<ab[1]<<" "<<ab[2]<<endl;
//            cout<<"bc: "<<bc[0]<<" "<<bc[1]<<" "<<bc[2]<<endl;
//            cout<<"ca: "<<ca[0]<<" "<<ca[1]<<" "<<ca[2]<<endl;
            QVector3D norm = QVector3D::crossProduct(ab,-ca);
            norm.normalize();
            //cout<<"norm: "<<norm[0]<<" "<<norm[1]<<" "<<norm[2]<<endl;
            //TriMesh::Point normal = cross(ab, -ca).normalize();
            double t = QVector3D::dotProduct(norm, p[0] - click_p) / QVector3D::dotProduct(norm, norm);
            //cout<<"t = "<<t<<endl;
            QVector3D project_p = click_p + t * norm;
            //TriMesh::Point project_p = click_p + t * normal;
            if( QVector3D::dotProduct(norm, QVector3D::crossProduct(ab, project_p - p[0])) >= 0 &&
                        QVector3D::dotProduct(norm, QVector3D::crossProduct(bc, project_p - p[1])) >= 0 &&
                        QVector3D::dotProduct(norm, QVector3D::crossProduct(ca, project_p - p[2])) >= 0 )
            {
                // yes
                double distance = (project_p - click_p).length();
                //cout<<"dist: " <<distance<<endl;
                if(distance < error)
                {
                    error = distance;
                    select_fh = f_it;
                }
            }
            else
            {
                // no
                continue;
            }
        }
        if(select_fh!=TriMesh::InvalidVertexHandle)
        {
            std::vector<TriMesh::FaceHandle>::iterator iter = std::find(mesh->select_mfh.begin(),mesh->select_mfh.end(),select_fh);
            if(iter != mesh->select_mfh.end())
            {
                //cout<<"face_delected!"<<endl;
                mesh->select_mfh.erase(iter);
                //Select_Invert_FaceBorder(select_fh);
            }
        }
        UpdateMesh();
    }

    void sel_Face(int x,int y)
    {
        makeCurrent();
        float depth_buffer;
        GLint V[4];
        glGetIntegerv(GL_VIEWPORT,V);
        glReadPixels( (int) x, (int) V[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,  &depth_buffer );
        cout<< depth_buffer<<endl;

        cout<<"View Port: "<<V[0]<<" "<<V[1]<<" "<<V[2]<<" "<<V[3]<<endl;

        QMatrix4x4 M,P;
        M.setToIdentity();
        P.setToIdentity();
        eye = QVector3D(0+e_x,step*-sin(deg_y)+e_y,step*cos(deg_y));
        center = QVector3D(e_x,e_y,0);
        up = QVector3D(0,cos(deg_y),0);
        M.lookAt(eye,center,up);
        M.rotate(deg_x,0,1,0);
        qreal zNear = 0.1, zFar = 10.0, fov = 45.0;
        P.perspective(fov, aspect, zNear, zFar);
        double objx,objy,objz;
        //cout<<x<<" "<<V[3]-y<<endl;
        unProject(x,V[3]-y,depth_buffer,M,P,V,&objx,&objy,&objz);

        //TriMesh::Point click_p(objx,objy,objz);
        QVector3D click_p = QVector3D(objx,objy,objz);
        double error = 1.0f;
        select_fh = TriMesh::InvalidFaceHandle;
        for(TriMesh::FIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it)
        {
            //TriMesh::Point p[3];
            QVector3D p[3];
            int i = 0;
            for(TriMesh::FVIter fv_it = mesh->fv_iter( *f_it ); fv_it; ++fv_it) {
                TriMesh::Point tem_p = mesh->point(fv_it);
                p[i++] = QVector3D(tem_p[0],tem_p[1],tem_p[2]);
            }
//            cout<<"p1"<<p[0].x()<<" "<<p[0].y()<<" "<<p[0].z()<<endl;
//            cout<<"p2"<<p[1].x()<<" "<<p[1].y()<<" "<<p[1].z()<<endl;
//            cout<<"p3"<<p[2].x()<<" "<<p[2].y()<<" "<<p[2].z()<<endl;
            //TriMesh::Point ab = p[1] - p[0], bc = p[2] - p[1], ca = p[0] - p[2];
            QVector3D ab = p[1] - p[0], bc = p[2] - p[1], ca = p[0] - p[2];
//            cout<<"ab: "<<ab[0]<<" "<<ab[1]<<" "<<ab[2]<<endl;
//            cout<<"bc: "<<bc[0]<<" "<<bc[1]<<" "<<bc[2]<<endl;
//            cout<<"ca: "<<ca[0]<<" "<<ca[1]<<" "<<ca[2]<<endl;
            QVector3D norm = QVector3D::crossProduct(ab,-ca);
            norm.normalize();
            //cout<<"norm: "<<norm[0]<<" "<<norm[1]<<" "<<norm[2]<<endl;
            //TriMesh::Point normal = cross(ab, -ca).normalize();
            double t = QVector3D::dotProduct(norm, p[0] - click_p) / QVector3D::dotProduct(norm, norm);
            //cout<<"t = "<<t<<endl;
            QVector3D project_p = click_p + t * norm;
            //TriMesh::Point project_p = click_p + t * normal;
            if( QVector3D::dotProduct(norm, QVector3D::crossProduct(ab, project_p - p[0])) >= 0 &&
                        QVector3D::dotProduct(norm, QVector3D::crossProduct(bc, project_p - p[1])) >= 0 &&
                        QVector3D::dotProduct(norm, QVector3D::crossProduct(ca, project_p - p[2])) >= 0 )
            {
                // yes
                double distance = (project_p - click_p).length();
                //cout<<"dist: " <<distance<<endl;
                if(distance < error)
                {
                    error = distance;
                    select_fh = f_it;
                }
            }
            else
            {
                // no
                continue;
            }
        }
        if(select_fh!=TriMesh::InvalidVertexHandle)
        {
            std::vector<TriMesh::FaceHandle>::iterator iter = std::find(mesh->select_mfh.begin(),mesh->select_mfh.end(),select_fh);
            if(iter == mesh->select_mfh.end())
            {
                //cout<<"face_selected!"<<endl;
                mesh->select_mfh.push_back(select_fh);
                //Select_Invert_FaceBorder(select_fh);
            }
        }

        UpdateMesh();
    }
    void paintGL()
    {
        // render something here

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
        if(indexSize > 0) {
            vbo.bind();
            vbo.allocate(vertexData.data(), vertexData.size() * sizeof(float));
            program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
            cbo.bind();
            cbo.allocate(colorData.data(), colorData.size() * sizeof(float));
            program.setAttributeBuffer(1, GL_FLOAT, 0, 3);

            nbo.bind();
            nbo.allocate(normalData.data(), normalData.size() * sizeof(float));
            program.setAttributeBuffer(2, GL_FLOAT, 0, 3);
            program.enableAttributeArray(0);
            program.enableAttributeArray(1);
            program.enableAttributeArray(2);
            ibo.bind();
            // Reset projection
            mvp.setToIdentity();
            model.setToIdentity();
            const qreal zNear = 0.1, zFar = 10.0, fov = 45.0;
            // Set perspective projection
            eye = QVector3D(0+e_x,step*-sin(deg_y)+e_y,step*cos(deg_y));
            center = QVector3D(e_x,e_y,0);
            up = QVector3D(0,cos(deg_y),0);
            mvp.perspective(fov, aspect, zNear, zFar);
            mvp.lookAt(eye,center,up);
            mvp.rotate(deg_x,0,1,0);
            model.rotate(deg_x,0,1,0);
            //mvp.rotate(deg_y,cos(-deg_x),0,-sin(-deg_x));
            //mvp.rotate(deg_y,1,0,0);
           // cout << deg_x <<" " <<deg_y<<endl;
            program.setUniformValue("mvp", mvp);
            program.setUniformValue("model_view", model);
            program.setUniformValue("M", model.normalMatrix());
            program.setUniformValue("eyepos",eye);
            if(render_mode == 0){ // solid
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
           }
            else if(render_mode == 1){ // wire
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
           }
            else if(render_mode == 2){ // solid wire
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(1.0, 1.0);
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
                glDisable(GL_POLYGON_OFFSET_FILL);
                cbo.bind();
                cbo.allocate(inv_colorData.data(), inv_colorData.size() * sizeof(float));
                program.setAttributeBuffer(1, GL_FLOAT, 0, 3);
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
           }

            program.disableAttributeArray(0);
            program.disableAttributeArray(1);
            program.disableAttributeArray(2);
        }
        if(silhouette_flag)
        {
            vector<float> sil_vertexData, sil_colorData;       
            for(int i = 0;i < mesh->sil_point.size();i+=2)
            {
                //cout<<cont++<<endl;
                sil_vertexData.clear();
                sil_colorData.clear();
                TriMesh::Point p = mesh->sil_point[i];
                sil_vertexData.push_back(p[0]);
                sil_vertexData.push_back(p[1]);
                sil_vertexData.push_back(p[2]);
                sil_colorData.push_back(1.0f);
                sil_colorData.push_back(0.0f);
                sil_colorData.push_back(0.0f);
                p = mesh->sil_point[i+1];
                sil_vertexData.push_back(p[0]);
                sil_vertexData.push_back(p[1]);
                sil_vertexData.push_back(p[2]);
                sil_colorData.push_back(1.0f);
                sil_colorData.push_back(0.0f);
                sil_colorData.push_back(0.0f);
                vbo.bind();
                vbo.allocate(sil_vertexData.data(), sil_vertexData.size() * sizeof(float));
                program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
                cbo.bind();
                cbo.allocate(sil_colorData.data(), sil_colorData.size() * sizeof(float));
                program.setAttributeBuffer(1, GL_FLOAT, 0, 3);
                program.enableAttributeArray(0);
                program.enableAttributeArray(1);
                glDrawArrays(GL_LINES, 0, 2);
                program.disableAttributeArray(0);
                program.disableAttributeArray(1);
            }
        }


        /**/
        /*
        glLineWidth( 10 );
        glBegin( GL_LINE_STRIP );
        float z = -50.0f;
        float angle = 0.0f;
        for ( angle = 0.0f; angle <= twoPi * 3.0f; angle += 0.1f, z += 0.5f )
        {
            float x = 50.0 * sin( angle );
            float y = 50.0 * cos( angle );
            glVertex3f( x, y, z );
        }
        glEnd();*/

        /*
        glBegin(GL_LINES);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex2d (cos(2), sin(2)); // x1,y1
        glVertex2d (m_x, m_y); // x2,y2
        */
        /*
         for( double i = 0 ; i < 2; i+=0.01 )
         {
           double value1 = cos(i) ;
           double value2 = sin(2) ;
           //double value2 = 5 ;
           //glColor3d( fabs(value1), fabs(value1)*fabs(value2), fabs(value2) );
           glColor3f(1.0f,0.0f,0.0f);
          // glVertex2d( .2*value1*value2,.4*value2 ) ;
           glVertex2d( value1, value2 ) ;
         }



        */

        /**/
        /*
        float depth_buffer;
        GLint V[4];
        glGetIntegerv(GL_VIEWPORT,V);
        glReadPixels( (int) m_x, (int) V[3]- m_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,  &depth_buffer );

        QMatrix4x4 M,P;
        M.setToIdentity();
        P.setToIdentity();
        eye = QVector3D(0+e_x,step*-sin(deg_y)+e_y,step*cos(deg_y));
        center = QVector3D(e_x,e_y,0);
        up = QVector3D(0,cos(deg_y),0);
        M.lookAt(eye,center,up);
        M.rotate(deg_x,0,1,0);
        qreal zNear = 0.1, zFar = 10.0, fov = 45.0;
        P.perspective(fov, aspect, zNear, zFar);


        double objx,objy,objz;
        unProject(m_x,V[3]- m_y,depth_buffer,M,P,V,&objx,&objy,&objz);


        cout<<"Cor: "<<objx<<" "<<objy<<" "<<objz<< endl;
        cout<<"Cor_ori: "<<m_x<<" "<<m_y<<endl;
        */


        /*以下程式碼為找出座標轉換的比例過程*/
        /*重要參數:
          1.mx為從模型座標轉成滑鼠座標的比例參數。
          2.mx_in為從滑鼠座標轉乘模型座標的比例參數。
          3.ox 和 oy 為視窗的中心點，每一台電腦的解析度不一，因此要執行這段程式碼。
          4.dx和dy為轉換後(滑鼠座標轉為模型座標)的結果
        */
        /*
        if(count == 1)
        {
            objx_one = objx;
            objy_one = objy;
        }

        else if(count == 2)
        {
            objx_two = objx;
            objy_two = objy;


            //float mx = abs(objx_one - objx_two ) / abs(m_x_one - m_x_two);
            float mx = abs(m_x_one - m_x_two) / abs(objx_one - objx_two );
            float my = abs(m_y_one - m_y_two) / abs(objy_one - objy_two );
            float mx_i = abs(objx_one - objx_two ) / abs(m_x_one - m_x_two);
            float my_i = abs(objy_one - objy_two ) / abs(m_y_one - m_y_two) ;



            cout<<"mx: "<<mx<<endl;
            cout<<"my: "<<my<<endl;

            float ox = m_x_one - mx * objx_one;
            float oy = m_y_one + my * objy_one;

            cout<<"ox: "<<ox<<endl;
            cout<<"oy: "<<oy<<endl;


            centerx = ox;
            centery = oy;

            dx = 0 + (m_x_two - centerx) * mx_i;
            dy = 0 + (centery - m_y_two) * my_i;

            cout<<"dx: "<<dx<<endl;
            cout<<"dy: "<<dy<<endl;


        }*/


        //dx = 0 + (m_x - centerx) * (x_y_in * 0.5);
        //dy = 0 + (centery - m_y) * (x_y_in * 0.5);

        //glClear (GL_COLOR_BUFFER_BIT);



        //list＜Coordinate＞::iterator c;

       for(list<Coordinate>::const_iterator c = coor.begin(); c!=coor.end(); ++c)
       {
           Coordinate another = *c;



           dx = 0 + (another.x - centerx) * (x_y_in * 0.4);
           dy = 0 + (centery - another.y) * (x_y_in * 0.4);

           cout<<"another.x: "<<another.x<<endl;
           cout<<"another.y: "<<another.y<<endl;

           glBegin (GL_POLYGON);
            for( double angle = 0 ; angle < 2*PI; angle+=0.01 )
            {
               // if(objz < 0)
                    //glVertex3f (objx + sin(angle) * Radius, objy + cos(angle) * Radius, objz + 4);

               // else
                    glColor3f(0.5,0.5,0.6);
                    glVertex3f (dx + sin(angle) * Radius, dy + cos(angle) * Radius, 1);
            }
           glEnd() ;

        }





        /**/

        glFinish();
    }

    void resizeGL(int w, int h)
    {
//        // adjust view transform here
//        float ratio = (float) w / (float) h;
//        // every call to member of QMatrix4x4 multiply upon its current matrix
//        mvp.setToIdentity();
//        mvp.ortho(-1 * ratio, 1 * ratio, -1, 1, 1, 100);
//        mvp.translate(0, 0, -50);
        // Calculate aspect ratio
        aspect = qreal(w) / qreal(h ? h : 1);

        // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
        const qreal zNear = 0.1, zFar = 5.0, fov = 45.0;

        // Reset projection
        mvp.setToIdentity();
        model.setToIdentity();
        // Set perspective projection
        mvp.perspective(fov, aspect, zNear, zFar);
        mvp.lookAt(eye, center, up);
    }
    void keyPressEvent(QKeyEvent *e){
        if(e->key() == Qt::Key_Shift)
        {
            shift_flag = 1;
        }
        if(e->key() == Qt::Key_Q)
        {
            //mesh->OMT_to_VF();
            mesh->IGL_Biharmonic_exe(deform_mode,0);
            UpdateMesh();
        }
        if(e->key() == Qt::Key_A)
        {
           // mesh->OMT_to_VF();

            mesh->IGL_Biharmonic_exe(deform_mode,1);
            UpdateMesh();
        }

        if(e->key() == Qt::Key_W)
        {
            //mesh->OMT_to_VF();
            mesh->IGL_Biharmonic_exe(deform_mode,2);
            UpdateMesh();
        }
        if(e->key() == Qt::Key_S)
        {
           // mesh->OMT_to_VF();

            mesh->IGL_Biharmonic_exe(deform_mode,3);
            UpdateMesh();
        }
        if(e->key() == Qt::Key_E)
        {
            //mesh->OMT_to_VF();
            mesh->IGL_Biharmonic_exe(deform_mode,4);
            UpdateMesh();
        }
        if(e->key() == Qt::Key_D)
        {
           // mesh->OMT_to_VF();

            mesh->IGL_Biharmonic_exe(deform_mode,5);
            UpdateMesh();
        }
        if(e->key() == Qt::Key_V)
        {
            mesh->IGL_Biharmonic_init();
            UpdateMesh();
        }
        if(e->key() == Qt::Key_C)
        {
            mesh->clear_select();
            UpdateMesh();
        }
        if(e->key() == Qt::Key_O)
        {
            mesh->saveObj();
        }
    }
    void keyReleaseEvent(QKeyEvent *e){
        if(e->key() == Qt::Key_Shift)
        {
            shift_flag = 0;
        }
    }
    void mousePressEvent(QMouseEvent *e){

        m_x = e->x();
        m_y = e->y();

        /*
        if(count == 1)
        {
            m_x_one = e->x();
            m_y_one = e->y();

            cout<<"mouse1: "<< m_x_one <<" "<< m_y_one <<endl;

        }
        else if(count == 2)
        {
            m_x_two = e->x();
            m_y_two = e->y();

            cout<<"mouse2: "<< m_x_two <<" "<< m_y_two <<endl;

        }*/

        if(shift_flag && e->buttons() == Qt::LeftButton)
        {
          // sel_Face(e->x(),e->y());
          // cout << m_x << " " << m_y << endl;
           repaint();

           count++;
        }

        else if(shift_flag && e->buttons() == Qt::RightButton)
            del_Face(e->x(),e->y());
       // cout<<"Press: "<<e->x()<<" "<<e->y()<<endl;
    }
    void mouseMoveEvent(QMouseEvent *e){
        if(!shift_flag){

            if(e->buttons() == Qt::LeftButton){
                deg_x += (e->x() - m_x)/5.0f;
                deg_y += (e->y() - m_y)/50.0f;
                repaint();
            }else if(e->buttons() == Qt::RightButton){
                e_x += (e->x() - m_x)/50.0f;
                e_y += (e->y() - m_y)/50.0f;
            }
            deg_x = deg_x > 180 ? deg_x - 360: deg_x;
            deg_x = deg_x < -180 ? deg_x + 360 : deg_x;
            deg_y = deg_y > 2*PI ? deg_y-2*PI : deg_y;
            deg_y = deg_y < -2*PI ? deg_y+2*PI : deg_y;
            if(e->x()!=m_x || e->y()!=m_y)
                repaint();
            m_x = e->x();
            m_y = e->y();
        }
        else{
            if(e->buttons() == Qt::LeftButton)
            {
                //sel_Face(e->x(),e->y());
                m_x = e->x();
                m_y = e->y();

                Coordinate cor;
                cor.x = m_x;
                cor.y = m_y;

                coor.push_back(cor);

                cout<<"size: "<<coor.size()<<endl;

                repaint();
            }
            else if(e->buttons() == Qt::RightButton)
                del_Face(e->x(),e->y());
        }
    }
    void wheelEvent(QWheelEvent *e){
        if(!shift_flag){
            QPoint numDegrees = e->angleDelta() / 8;
            step += numDegrees.y() / 100.0f;
            repaint();
        }
    }

private:
    QOpenGLBuffer vbo, ibo, cbo, nbo;
    QOpenGLShaderProgram program;
    QMatrix4x4 mvp,model;
    LightSourceObject *dir_light;
    TriMesh::FaceHandle select_fh;
    vector<float> vertexData, colorData,normalData,inv_colorData;
    vector<unsigned int> indexData;
    //vector<TriMesh::FaceHandle> select_mfh;
    int indexSize;
    float m_x;
    float m_y;

    float m_x_one, m_y_one, m_x_two, m_y_two;

    float count;

    float objx_one,objy_one, objx_two, objy_two;

    float centerx, centery;

    float dx, dy;
    float x_y, x_y_in;

   // QColor faceColors[4];
    list<Coordinate> coor;


    float diff;
    float step;
    float deg_x,deg_y;
    int width,height;
    float e_x,e_y;
    int shift_flag;
    int silhouette_flag;
    int render_mode;
    int deform_mode;
    QMatrix4x4 projection;
    QVector3D eye,center,up;
    QVector3D click_point;
    qreal aspect;
    float p[4],a[4],d[4],s[4];

};

#endif // MYOPENGLWIDGET

