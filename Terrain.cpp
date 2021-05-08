#include "Terrain.h"

RawModel Terrain::generateTerrain(ModelLoader& loader) {
 

  const int count = VERTEX_COUNT * VERTEX_COUNT;

  float * vertices = new float[count * 3];
  float * normals = new float[count * 3];
  float * textureCoords = new float[count * 2];
  int * indices = new int[6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1)];

  // Vertex pointer
  int vp = 0;

  for(int i = 0; i < VERTEX_COUNT; i++) {
    for(int j = 0; j < VERTEX_COUNT; j++) {
      vertices[vp * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
      vertices[vp * 3 + 1] = 0;
      vertices[vp * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
      normals[vp * 3] = 0;
      normals[vp * 3 + 1] = 1;
      normals[vp * 3 + 2] = 0;
      textureCoords[vp * 2] = (float)j / ((float)VERTEX_COUNT - 1);
      textureCoords[vp * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
      vp++;
    }
  }

  int pointer = 0;
  for(int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
    for(int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
      int topLeft = (gz * VERTEX_COUNT) + gx;
      int topRight = topLeft + 1;
      int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
      int bottomRight = bottomLeft + 1;
      indices[pointer++] = topLeft;
      indices[pointer++] = bottomLeft;
      indices[pointer++] = topRight;
      indices[pointer++] = topRight;
      indices[pointer++] = bottomLeft;
      indices[pointer++] = bottomRight;
    }
  }

  RawModel model = loader.loadRaw(vertices, count * 3, indices, 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1), textureCoords, count * 2, normals, count * 3);
  delete[] vertices;
  delete[] textureCoords;
  delete[] normals;
  delete[] indices;
  return model;
}
