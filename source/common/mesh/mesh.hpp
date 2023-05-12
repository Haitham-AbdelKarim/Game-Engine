#pragma once

#include "vertex.hpp"
#include <glad/gl.h>

namespace our {

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

class Mesh {
  // Here, we store the object names of the 3 main components of a mesh:
  // A vertex array object, A vertex buffer and an element buffer
  unsigned int VBO, EBO;
  unsigned int VAO;
  // We need to remember the number of elements that will be draw by
  // glDrawElements
  GLsizei elementCount;

public:
  // The constructor takes two vectors:
  // - vertices which contain the vertex data.
  // - elements which contain the indices of the vertices out of which each
  // rectangle will be constructed. The mesh class does not keep a these data on
  // the RAM. Instead, it should create a vertex buffer to store the vertex data
  // on the VRAM, an element buffer to store the element data on the VRAM, a
  // vertex array object to define how to read the vertex & element buffer
  // during rendering
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &elements) {
    // TODO: (Req 2) Write this function
    //  remember to store the number of elements in "elementCount" since you
    //  will need it for drawing For the attribute locations, use the constants
    //  defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
    elementCount = (GLsizei)elements.size(); // store the number of elements
    glGenBuffers(1, &VBO);                   // create a new buffer for VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);      // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW); // initialize the buffer

    glGenBuffers(1, &EBO);                      // create a new buffer for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(uint32_t),
                 &elements[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);         // create a new buffer for VAO
    glBindVertexArray(VAO);             // Bind VAO as the vertix array
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO

    glEnableVertexAttribArray(ATTRIB_LOC_POSITION); // Enable position attribute
    glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false,
                          sizeof(Vertex),
                          0); // set the pointer of the position attribute

    glEnableVertexAttribArray(ATTRIB_LOC_COLOR); // Enable color attribute
    glVertexAttribPointer(
        ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex),
        (void *)offsetof(Vertex,
                         color)); // set the pointer of the color attribute

    glEnableVertexAttribArray(
        ATTRIB_LOC_TEXCOORD); // Enable texture coordinate attribute
    glVertexAttribPointer(
        ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex),
        (void *)offsetof(
            Vertex,
            tex_coord)); // set the pointer of the texture coordinate attribute

    glEnableVertexAttribArray(ATTRIB_LOC_NORMAL); // Enable normal attribute
    glVertexAttribPointer(
        ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex),
        (void *)offsetof(Vertex,
                         normal)); // set the pointer of the normal attribute

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 EBO); // Bind the element buffer object as the element array

    glBindVertexArray(0); // unbind the vertix array object
  }
  Mesh(Mesh & copied)
  {
   // rigidbody= new reactphysics3d::RigidBody(*copied.rigidbody);
    VBO = copied.VBO;
    EBO=copied.EBO;
    VAO=copied.VAO;
    elementCount=copied.elementCount;
  }

  // this function should render the mesh
  void draw() {
    // TODO: (Req 2) Write this function
    glBindVertexArray(VAO); //Bind the vertix array object
    glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0); //Draw the elements
  }

  // this function should delete the vertex & element buffers and the vertex
  // array object
  ~Mesh() {
    // TODO: (Req 2) Write this function
    glDeleteBuffers(1, &VBO); //Delete the vertix buffer object
    glDeleteBuffers(1, &EBO); // Delete the elements buffer object
    glDeleteVertexArrays(1, &VAO); // Delete the vertix array object
  }

  Mesh(Mesh const &) = delete;
  Mesh &operator=(Mesh const &) = delete;
};

} // namespace our