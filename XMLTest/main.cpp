#include <ostream>
#include <ngl/Vec4.h>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Mat4.h>
#include <ngl/Mat3.h>
#include <ngl/Material.h>
#include <ngl/Quaternion.h>
#include <ngl/Plane.h>
#include <ngl/SpotLight.h>
#include <ngl/XMLSerializer.h>


int main(int argc, char **argv)
{
  ngl::Vec4 vec4(2.2,3.4,2.0,1.0);
  ngl::Vec3 vec3(1,2,3);
  ngl::Vec2 vec2(99,99);

  ngl::XMLSerializer doc("xmlTest.xml",ngl::XMLSerializer::WRITE);


  doc.addNode("SceneNode");
  doc.addNode("Position",doc.getCurrentNode());
  doc.write(vec4,doc.getCurrentNode(),"vec4");

  doc.addNode("Rotation");
  doc.write(vec4,doc.getCurrentNode(),"vec4");

 /* doc.write(vec4,"vec4");
  doc.write(vec3,"vec3");
  doc.write(vec2,"vec2");
  ngl::Colour c(0.2f,0.03f,0.2f,0.1f);
  doc.write(c,"Colour");
  ngl::Material m(ngl::GOLD);
  doc.write(m,"material");
  ngl::Mat4 m4;
  m4.rotateX(45);
  doc.write(m4,"mat4");
  ngl::Mat3 m3;
  m3.rotateX(45);
  doc.write(m3,"mat3");
  ngl::Transformation tx;
  tx.setPosition(2,3,2);
  tx.setRotation(45,34,22);
  tx.setScale(0.2,2.0,0.2);
  doc.write(tx,"transformation");
  ngl::Quaternion q(m4);
  doc.write(q,"quaternion");
  ngl::Camera cam(ngl::Vec3(2,2,2),ngl::Vec3(0,0,0),ngl::Vec3(0,1,0));
  doc.write(cam,"Camera");
  ngl::Light l1(ngl::Vec3(2,2,2),c,ngl::POINTLIGHT);
  doc.write(l1,"L1");

  /* // Write xml file =================================
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(doc.allocate_attribute("encoding", "us-ascii"));
  doc.append_node(decl);





  ngl::Light l1(ngl::Vec3(2,2,2),c,ngl::POINTLIGHT);
  l1.writeXML(doc,"L1");

  ngl::SpotLight spot(ngl::Vec3(3,4,3),ngl::Vec3(0,0,0),c);
  spot.writeXML(doc,"spot");

  ngl::Plane plane(ngl::Vec3(-1,0,0),ngl::Vec3(0,1,0),ngl::Vec3(1,0,0));
  plane.writeXML(doc,"plane");


  std::ofstream file_stored("xmlTest.xml");
  file_stored << doc;
  file_stored.close();
  doc.clear();



  std::cout<<"Reading \n";
  rapidxml::xml_node<> * rootNode;
  // Read the xml file into a vector
  std::ifstream theFile ("Shark.xml");
  std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  buffer.push_back('\0');
  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&buffer[0]);
//  for(  rootNode = doc.first_node(); rootNode; rootNode = rootNode->next_sibling() )
//  {
//    std::cout<<rootNode->name()<<"\n";
//      std::cout<<rootNode->value()<<"\n";
//  }
  rootNode=doc.first_node("NCCAPointBake");
  rapidxml::xml_node<> * child=rootNode->first_node("MeshName");
  std::cout<<child->value()<<"\n";

  child=rootNode->first_node("StartFrame");
  std::cout<<child->value()<<"\n";
  child=rootNode->first_node("EndFrame");
  std::cout<<child->value()<<"\n";
  child=rootNode->first_node("NumFrames");
  std::cout<<child->value()<<"\n";
  std::string v(child->value());
  boost::algorithm::trim(v);
  int ivalue=boost::lexical_cast<int>(v);
  std::cout<<"v is"<<v<<"\n";
  std::cout<<"worked "<<ivalue<<"\n";

  for(child=rootNode->first_node("Frame"); child; child=child->next_sibling())
  {
    std::cout<< child->first_attribute("number")->value()<<"\n";
    for(rapidxml::xml_node<> * vertex=child->first_node("Vertex"); vertex; vertex=vertex->next_sibling())
    {
      std::cout<<vertex->value()<<"\n";
    }

  }
*/
}
