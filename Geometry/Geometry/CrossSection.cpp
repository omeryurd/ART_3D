#include "CrossSection.h"

#define epsilon (DBL_MIN * 10000)

using namespace MTF;

CrossSection::CrossSection(std::vector<Face*> _faces, Plane *_plane, int _size, int _xPos, int _yPos)
{
    faces = _faces;
    plane = _plane;
    size = _size;
    xPos = _xPos;
    yPos = _yPos;

    CreateTexture();
}


CrossSection::~CrossSection()
{
    glDeleteTextures(1, &texture);
}


void CrossSection::SetPlane(Plane *_plane)
{
    plane = _plane;
}


Plane* CrossSection::GetPlane()
{
    return plane;
}


void CrossSection::Render()
{
    // Render the plane that creates the cross section
    //plane->Render();

    // Render the cross section area
    // First push the current view and projection matrix to store them while loading
    // the identity matrix in each (makes projection to cover the screen much easier)
    glMatrixMode(GL_MODELVIEW); 
    glPushMatrix(); 
    glLoadIdentity(); 
    glMatrixMode(GL_PROJECTION); 
    glPushMatrix(); 
    glLoadIdentity();

    // Enable our texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Find the location for the texture
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float screenWidth = viewport[2];
    float screenHeight = viewport[3];
    float startX = (screenWidth - xPos - size - 50) / screenWidth * 2 - 1;
    float startY = (screenHeight / 2 - yPos - size / 2) / screenHeight * 2 - 1;
    float endX = startX + (size / screenWidth * 2);
    float endY = startY + (size / screenHeight * 2);

    // Render the texture
    glBegin(GL_QUADS); 
        //glVertex3i(-1, -1, -1);
        glTexCoord2f(0, 0);
        glVertex3f(startX, startY, -1);
        
        //glVertex3i( 1, -1, -1);
        glTexCoord2f(1, 0);
        glVertex3f(endX, startY, -1);

        //glVertex3i( 1,  1, -1); 
        glTexCoord2f(1, 1);
        glVertex3f(endX,  endY, -1); 

        //glVertex3i(-1,  1, -1);
        glTexCoord2f(0, 1);
        glVertex3f(startX,  endY, -1); 
    glEnd();

    // Restore the projection and view matrix
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


Vector3 CrossSection::GetCenter()
{
    return plane->GetCenter();
}


void CrossSection::SetCenter(Vector3 _center)
{
    plane->SetCenter(_center);
}


double CrossSection::GetPitch()
{
    return plane->GetPitch();
}


void CrossSection::SetPitch(double _pitch)
{
    plane->SetPitch(_pitch);
}


double CrossSection::GetYaw()
{
    return plane->GetYaw();
}


void CrossSection::SetYaw(double _yaw)
{
    plane->SetYaw(_yaw);
}


double CrossSection::GetRadius()
{
    return plane->GetRadius();
}


void CrossSection::SetRadius(double _radius)
{
    plane->SetRadius(_radius);
}


void CrossSection::CalculateCrossSection()
{   
    // This rot matrix is used to rotate points on an arbitrary plane to be in the xy plane
    double invRotArray[16];
    Matrix4 invRot = plane->GetRotationMatrix().GetInversion();
    invRot.GetMatrixArray(invRotArray);

    // Find the line segments that make up the cross section
    std::vector<LineSegment*> lineSegments;

    int numIntersection = 0;
    int numFaces = faces.size();

    for (int i = 0; i < numFaces; ++i)
    {
        LineSegment *segment = new LineSegment(Vector3::ZERO, Vector3::ZERO);
        if (plane->GetIntersectionLineSegment(*faces.at(i), *segment))
        {
            lineSegments.push_back(segment);
            ++numIntersection;
        }
    }

    // Find the min and max points (useful for centering the cross section image)
    double xMin, xMax, yMin, yMax;
    xMin = -5; xMax = 5;
    yMin = -5; yMax = 5;

    double xWidth = xMax - xMin;
    double yWidth = yMax - yMin;
    double xCenter = (xMax + xMin) / 2;
    double yCenter = (yMax + yMin) / 2;
    double projWidth = max(xWidth, yWidth) * 0.55;

    // Remove segments that are the same point
    bool dstop = false;
    while (!dstop && !lineSegments.empty())
    {
        dstop = true;
        for (int i = 0; i < lineSegments.size(); ++i)
        {
            LineSegment *curSeg = lineSegments.at(i);
            if (curSeg->point1 == curSeg->point2)
            {
                lineSegments.erase(lineSegments.begin() + i);
                dstop = false;
                break;
            }
        }
    }

    // Take the individual line segments and create a list of points in the correct order
    // We need to take into account that there may be multiple polygons in our set of points
    // This can be caused by the implant inside the model, or if the cutting plane is rotated
    // It may intersect distinct parts of the bone that are not combined in the plane
    // We will solve this by ignoring any polygon we do not start with.  This is not the best solution
    std::vector<Vector3> outlineVector;
    if (!lineSegments.empty())
    {
        outlineVector.push_back(lineSegments.at(0)->point1);
        outlineVector.push_back(lineSegments.at(0)->point2);
        lineSegments.erase(lineSegments.begin());

        bool stop = false;
        while (!stop && !lineSegments.empty())
        {
            stop = true;
            Vector3 target = outlineVector.at(outlineVector.size() - 1);

            for (int i = 0; i < lineSegments.size(); ++i)
            {
                LineSegment *curSeg = lineSegments.at(i);
                if (curSeg->point1 == target)
                {
                    outlineVector.push_back(curSeg->point2);
                    lineSegments.erase(lineSegments.begin() + i);
                    stop = false;
                    break;
                }
                else if (curSeg->point2 == target)
                {
                    outlineVector.push_back(curSeg->point1);
                    lineSegments.erase(lineSegments.begin() + i);
                    stop = false;
                    break;
                }
            }

            if (stop)
            {
                std::cout << "Error: " << std::endl;
                std::cout << "Target: " << target.ToString();
                std::cout << "Remaining: " << std::endl;
                for (int i = 0; i < lineSegments.size(); ++i)
                {
                    std::cout << "P1: " << lineSegments.at(i)->point1.ToString();
                    std::cout << "P2: " << lineSegments.at(i)->point2.ToString();
                }
                std::cout << std::endl;
            }
        }

        // Remove the last point, as it duplicates the first
        outlineVector.erase(outlineVector.end() - 1);
        outlineVector.shrink_to_fit();
    }

    // Create two arrays to store the points that make up the outline of the polygon
    // Also find the min and max in the x and y directions
    int polygonLength = outlineVector.size();
    double *polygonX = new double[polygonLength];
    double *polygonY = new double[polygonLength];
    
    for (int i = 0; i < polygonLength; ++i)
    {
        Vector3 v = invRot * outlineVector.at(i);
        polygonX[i] = v.GetX();
        polygonY[i] = v.GetY();
    }

    // Set the render target to the texture
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, textureFBO);
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, size, size);

    // Save the current view and projection matrix
    glMatrixMode(GL_MODELVIEW); 
    glPushMatrix(); 
    glLoadIdentity();
    gluLookAt(xCenter, yCenter, -3.0, xCenter, yCenter, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-projWidth, projWidth, -projWidth, projWidth, 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);

    // Update the texture
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    //

    glClear(GL_COLOR_BUFFER_BIT);

    glColor4f(0.0, 1.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < polygonLength; ++i)
    {
        glVertex2f(polygonX[i], polygonY[i]);
    }
    glEnd();

    // Restore the projection and view matrix
    glMatrixMode(GL_PROJECTION); 
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // We no longer want to use the texture as the render target
    glPopAttrib();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    // Cleanup - we dont' want any memory leaks
    delete polygonX;
    delete polygonY;
}


void CrossSection::CreateTexture()
{
    GLubyte *data = new GLubyte[size * size * 4];

    int pixelIndex = 0;

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            for (int channel = 0; channel < 4; channel++)
            {
                data[pixelIndex] = 200;
                ++pixelIndex;
            }
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Setup texture attributes
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Setup our render buffers so we can render to a texture
    /**/
    glGenFramebuffersEXT(1, &textureFBO);
    glGenRenderbuffersEXT(1, &textureDepthBuffer);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, textureFBO);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, textureDepthBuffer);

    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, size, size);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, textureDepthBuffer);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);

    if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        std::cout << "Frame buffer is not setup correctly, GL ERROR: " << glGetError() << " | " << glewGetErrorString(glGetError()) << std::endl;
        exit(-1);
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    /**/

    std::cout << "Texture and render targets setup correctly!" << std::endl;

    delete data;
}

/*
bool CrossSection::IsPointInPolygon(double x, double y, double *polygonX, double *polygonY, int length)
{
    int j = length - 1 ;
    boolean oddNodes = false;

    for (int i = 0; i < length; ++i) 
    {
        if ((polygonY[i] < y && polygonY[j] >= y || polygonY[j] < y && polygonY[i] >= y) && (polygonX[i] <= x || polygonX[j] <= x)) 
        {
            if (polygonX[i] + (y-polygonY[i]) / (polygonY[j] - polygonY[i]) * (polygonX[j] - polygonX[i]) < x) 
            {
                oddNodes = !oddNodes; 
            }
        }
        j=i; 
    }

    return oddNodes; 
}
/**/

bool CrossSection::IsPointInPolygon(double x, double y, double *polygonX, double *polygonY, int length) 
{

    int j = length - 1;
    bool oddNodes = false;

    for (int i = 0; i < length; i++)
    {
        if (polygonY[i] < y && polygonY[j] >= y || polygonY[j] < y && polygonY[i] >= y) 
        {
            if (polygonX[i] + (y - polygonY[i]) / (polygonY[j] - polygonY[i]) * (polygonX[j] - polygonX[i]) < x) 
            {
                oddNodes = !oddNodes; 
            }
        }
        j = i; 
    }

    return oddNodes; 
}


void CrossSection::SetFaces(std::vector<Face*> _faces)
{
    faces = _faces;
    CalculateCrossSection();
}
