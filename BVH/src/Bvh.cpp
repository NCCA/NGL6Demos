#include "Bvh.h"
#include <cstdlib>

// add the bvh filename as the commandline parameter to the main program
// interaction:
//      1. press "r" to replay the animation from beginning
//      2. press "p" to pause and again to continue the play
//      3. press right arrow to forward one frame while pausing the play
//      4. press left arrow to backward one frame while pausing the play

// workshop:
//    1.

Bvh::Bvh(const std::string &_bvhFileName, Scene *_parent)
{
    m_parent=_parent;
    m_bvhFileName = _bvhFileName;
    m_currentFrames = 0;
    m_heightOfRoot = 0;
    load();
}

Bvh::~Bvh()
{
}

void Bvh::drawBones(Joint *_currentJoint, ngl::Mat4 _parentM) const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglDiffuseShader"]->use();

    ngl::Mat4 cameraM = m_parent->getCamera()->getVPMatrix();
    ngl::Mat4 MVP=_parentM*cameraM;
    ngl::Mat3 normalM = MVP;
    normalM.inverse();
    shader->setRegisteredUniform("MVP",MVP);
    shader->setRegisteredUniform("normalMatrix",normalM);

    // get an instance of the VBO primitives for drawing
    ngl::VAOPrimitives::instance()->draw("sphere");

    ngl::Mat4 rotX, rotY, rotZ, currentOrientM;
    // draw subtree of this joint
    if(_currentJoint->children.size() != 0)
    {
        for(unsigned int i=0;i<3;i++)
        {
            switch(_currentJoint->m_rotate_order[i])
            {
            case 0: rotX.rotateX(_currentJoint->m_rotate[0][m_currentFrames]);
                currentOrientM *= rotX;
                break;
            case 1: rotY.rotateY(_currentJoint->m_rotate[1][m_currentFrames]);
                currentOrientM *= rotY;
                break;
            case 2: rotZ.rotateZ(_currentJoint->m_rotate[2][m_currentFrames]);
                currentOrientM *= rotZ;
                break;
            }
        }
    }

    ngl::Transformation childScaleTrans;
    for(std::vector<Joint *>::iterator itr = _currentJoint->children.begin(); itr!=_currentJoint->children.end(); ++itr)
    {

        float offsetDist = (*itr)->m_offset.length();
        ngl::Mat4 orientM = getRotationFromZ((*itr)->m_offset);

        childScaleTrans.setScale(1.0, 1.0, offsetDist);
        ngl::Mat4 scaleM = childScaleTrans.getMatrix();

        MVP=scaleM*orientM*currentOrientM*_parentM*cameraM;
        shader->setShaderParamFromMat4("MVP",MVP);
        // get an instance of the VBO primitives for drawing
        ngl::VAOPrimitives *prim1=ngl::VAOPrimitives::instance();
        // draw the Bone
        prim1->draw("cylinder");

        ngl::Mat4 childOffsetM, childM;

        childOffsetM.m_m[3][0] = (*itr)->m_offset.m_x;
        childOffsetM.m_m[3][1] = (*itr)->m_offset.m_y;
        childOffsetM.m_m[3][2] = (*itr)->m_offset.m_z;
        childM = childOffsetM * currentOrientM * _parentM;
        drawBones(*itr, childM);
    }
}

ngl::Mat4 Bvh::getRotationFromZ(ngl::Vec3 _vec) const
{
    ngl::Mat4 rotM;
    float x, y, z;
    // rotate negative z axis to _vec direction
    _vec.normalize();
    ngl::Vec3 nz(0,0,-1);
    float angle = acos(_vec.dot(nz));
    ngl::Vec3 norm = _vec.cross(nz);
    if(norm.length()<= 0.0001)
    {
        x= z = 0.0;
        y = 1.0;
    }
    else
    {
        norm.normalize();
        x = norm.m_x;
        y = norm.m_y;
        z = norm.m_z;
    }

    // Axis and Angle matrix rotation see
    // http://en.wikipedia.org/wiki/Rotation_matrix for more details
    float c = cos(angle);
    float s = sin(angle);
    float C=1-c;
    float xs  = x*s;  float  ys = y*s;  float  zs = z*s;
    float xC  = x*C;  float  yC = y*C;  float  zC = z*C;
    float xyC = x*yC; float yzC = y*zC; float zxC = z*xC;

    rotM.m_m[0][0]=x*xC+c;  rotM.m_m[0][1]= xyC-zs;  rotM.m_m[0][2]= zxC+ys;
    rotM.m_m[1][0]=xyC+zs;   rotM.m_m[1][1]=y*yC+c;  rotM.m_m[1][2]= yzC-xs;
    rotM.m_m[2][0]=zxC-ys;   rotM.m_m[2][1]=yzC+xs;  rotM.m_m[2][2]=z*zC+c;
    return rotM;
}

void Bvh::draw() const
{
    ngl::Mat4 currentM;
    currentM.m_m[3][0] = -m_root->m_offset.m_x+m_root_translation[0][m_currentFrames];
    currentM.m_m[3][1] = -m_root->m_offset.m_y+m_root_translation[1][m_currentFrames]-m_heightOfRoot;
    currentM.m_m[3][2] = -m_root->m_offset.m_z+m_root_translation[2][m_currentFrames];

    ngl::Mat4 globalM = m_parent->getTransformStack()->getGlobalTransform().getMatrix();
    currentM = currentM*globalM;
    drawBones(m_root, currentM);

}

void Bvh::replay()
{
    m_currentFrames=0;
}

void Bvh::update()
{
    if(m_currentFrames>=m_numFrames-1)
        m_currentFrames=0;
    else
        m_currentFrames++;
}

void Bvh::forwardAnimation()
{
    if(m_currentFrames>=m_numFrames-1)
        return;
    else
        m_currentFrames++;
}

void Bvh::backwardAnimation()
{
    if(m_currentFrames>0)
        m_currentFrames--;
}

std::vector<std::string> Bvh::tokenize(const std::string &str, const std::string &delim)
{
    std::vector<std::string> tokens;
    size_t p0 = 0, p1 = std::string::npos;

    while(p0 != std::string::npos)
    {
        p1 = str.find_first_of(delim, p0);
        if(p1!=p0)
        {
            if(p1 != std::string::npos)
            {
                std::string token = str.substr(p0, p1 - p0);
                tokens.push_back(token);
            }
            else
            {
                std::string token = str.substr(p0, std::string::npos);
                tokens.push_back(token);
                return tokens;
            }
        }
        p0 = str.find_first_not_of(delim, p1);
    }
    return tokens;
}

bool Bvh::load()
{
    std::string keywords_str[]={"HIERARCHY", "ROOT", "JOINT", "OFFSET", "CHANNELS", "End","MOTION", "Frames:", "Frame", "{", "}"};
    std::ifstream in(m_bvhFileName.c_str());
    if (in.is_open() != true)
    {
        std::cout<<"FILE NOT FOUND !!!! "<<m_bvhFileName.c_str()<<"\n";
        return false;
    }
    std::string str;

    // loop grabbing a line and then pass it to our parsing framework
    unsigned int                numKeywords = sizeof(keywords_str)/sizeof(std::string);
    unsigned int                i, numChannels, numDOF=0;
    std::vector<std::string>    inputTokens;
    Joint                       *currentJoint, *parentJoint;
    std::stack<Joint *>         jointStack;
    std::vector<Joint *>        jointArray; // follow the order of the animation data, doesn't include root joint
    bool                        reading_structure=true;
    float                       currentHeight=0.0;

    while(getline(in, str)&&reading_structure)
    {
        inputTokens = tokenize(str, " \t\n\r");
        for(i=0;i<numKeywords;++i)
        {
            if(inputTokens[0].compare(keywords_str[i])==0)
            {
                if(i==5)
                {
                    if(inputTokens[1].compare("Site")==0)
                        break;
                }
                else if(i==8)
                {
                    if(inputTokens[1].compare("Time:")==0)
                        break;
                }
                else
                    break;
            }
        }
        if(i>=numKeywords) // no match of this line
        {
            std::cout<<"something unexpected with this bvh file\n" << str << "\n";
            return false;
        }
        switch (i)
        {
        case HIERARCHY:
            break;
        case ROOT:
            currentJoint = m_root = new Joint;
            currentJoint->m_name = inputTokens[1];
            break;
        case LEFTPARENTHESES:
            jointStack.push(currentJoint);
            break;
        case RIGHTPARENTHESES:
            currentHeight -= currentJoint->m_offset[1];
            currentJoint = jointStack.top();
            jointStack.pop();
            if(jointStack.size()==0)
                reading_structure=false;
            break;
        case CHANNELS:
            numChannels = atol(inputTokens[1].c_str());
            if(numChannels == 6) // root joint
            {
                for(i=0;i<3;i++)
                {
                    if(inputTokens[i+2].compare("Xposition")==0) m_root_translation_order[i] = 0;
                    if(inputTokens[i+2].compare("Yposition")==0) m_root_translation_order[i] = 1;
                    if(inputTokens[i+2].compare("Zposition")==0) m_root_translation_order[i] = 2;
                }
                for(i=0;i<3;i++)
                {
                    if(inputTokens[i+5].compare("Xrotation")==0) currentJoint->m_rotate_order[i] = 0;
                    if(inputTokens[i+5].compare("Yrotation")==0) currentJoint->m_rotate_order[i] = 1;
                    if(inputTokens[i+5].compare("Zrotation")==0) currentJoint->m_rotate_order[i] = 2;
                }
                numDOF+=6;
            }
            else if(numChannels == 3)
            {
                jointArray.push_back(currentJoint);
                for(i=0;i<3;i++)
                {
                    if(inputTokens[i+2].compare("Xrotation")==0) currentJoint->m_rotate_order[i] = 0;
                    if(inputTokens[i+2].compare("Yrotation")==0) currentJoint->m_rotate_order[i] = 1;
                    if(inputTokens[i+2].compare("Zrotation")==0) currentJoint->m_rotate_order[i] = 2;
                }
                numDOF+=3;
            }
            else
            {
                std::cout<< "the number of Channels is not right\n";
                return false;
            }
            break;
        case JOINT:
            currentJoint = new Joint;
            currentJoint->m_name = inputTokens[1];
            parentJoint = jointStack.top();
            parentJoint->children.push_back(currentJoint);
            break;
        case END_SITE:
            currentJoint = new Joint;
            parentJoint = jointStack.top();
            parentJoint->children.push_back(currentJoint);
            break;
        case OFFSET:
            currentJoint->m_offset[0] = strtod(inputTokens[1].c_str(), NULL)/10.0;
            currentJoint->m_offset[1] = strtod(inputTokens[2].c_str(), NULL)/10.0;
            currentJoint->m_offset[2] = strtod(inputTokens[3].c_str(), NULL)/10.0;
            if(currentJoint != m_root)
            {
                currentHeight += currentJoint->m_offset[1];
                if(m_heightOfRoot>currentHeight)
                    m_heightOfRoot = currentHeight;
            }
            break;
        }
    }

    // get to the "Motion" line
    inputTokens = tokenize(str, " \t\n\r");
    if(inputTokens[0].compare("MOTION")!=0)
    {
        std::cout<<"unexpeced line: " << str << "\n";
        return false;
    }

    // get to the frame number line
    getline(in, str);
    inputTokens = tokenize(str, " \t\n\r");
    if(inputTokens[0].compare("Frames:")!=0)
    {
        std::cout<<"unexpeced line: " << str << "\n";
        return false;
    }
    else
        m_numFrames=atol(inputTokens[1].c_str());

    // get to the frame time line
    getline(in, str);
    inputTokens = tokenize(str, " \t\r\n");
    if(inputTokens[0].compare("Frame")!=0 || inputTokens[1].compare("Time:")!=0)
    {
        std::cout<<"unexpeced line: " << str << "\n";
        return false;
    }
    else
    {
        m_frameTime = strtod(inputTokens[2].c_str(), NULL);
        m_frameTime *= 1000; // turn the seconds to macroseconds
    }

    // error checking
    unsigned int numJoint = jointArray.size(); // not include root
    if(numDOF!=numJoint*3+6)
    {
        std::cout<< "something wrong during the reading\n";
        return false;
    }

    // read animation data
    unsigned lineCount = 0;
    float *frameData = new float[numDOF];

    while(getline(in, str)&&lineCount < m_numFrames)
    {
        inputTokens = tokenize(str, " \t\n\r");
        for(i=0;i<numDOF;i++)
            frameData[i] = strtod(inputTokens[i].c_str(), NULL);
        m_root_translation[m_root_translation_order[0]].push_back(frameData[0]/10.0);
        m_root_translation[m_root_translation_order[1]].push_back(frameData[1]/10.0);
        m_root_translation[m_root_translation_order[2]].push_back(frameData[2]/10.0);
        m_root->m_rotate[m_root->m_rotate_order[0]].push_back(frameData[3]);
        m_root->m_rotate[m_root->m_rotate_order[1]].push_back(frameData[4]);
        m_root->m_rotate[m_root->m_rotate_order[2]].push_back(frameData[5]);
        for(i=0;i<numJoint;i++)
        {
            jointArray[i]->m_rotate[jointArray[i]->m_rotate_order[0]].push_back(frameData[i*3+6]);
            jointArray[i]->m_rotate[jointArray[i]->m_rotate_order[1]].push_back(frameData[i*3+7]);
            jointArray[i]->m_rotate[jointArray[i]->m_rotate_order[2]].push_back(frameData[i*3+8]);
        }
        lineCount++;
    }
    if(lineCount < m_numFrames)
    {
        std::cout << " not enough frame data in the file, " << (m_numFrames-lineCount) << " more lines expected\n";
        return false;
    }
    delete frameData;
    jointArray.clear();

    // now we are done close the file
    in.close();
    return true;
}
