void test_DrawAxes(RenderDC* rDC)
{
  //g3dDisable(GL_LIGHTING);
  //g3dMatrixMode(GL_MODELVIEW);
  //g3dPushMatrix();
  //g3dScalef(1.4f, 1.4f, 1.4f);
  g3dBegin(rDC, G3D_LINES, 6);
  // x axis
  g3dColor3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.0f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  // y axis
  g3dColor3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.0f, 0.0f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  // z axis
  g3dColor3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.0f, 0.0f, 0.0f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dEnd();
  g3dBegin(rDC, G3D_TRIANGLES, 150);
  // x axis arrow
  g3dColor3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, 0.0f, 0.04f);
  g3dVertex3f(0.9f, 0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.04f);
  g3dVertex3f(0.9f, 0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, 0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.04f, 0.0f);
  g3dVertex3f(0.9f, 0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.04f, 0.0f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, 0.04f, 0.0f);
  g3dVertex3f(0.9f, 0.028f, -0.028f);
  g3dVertex3f(0.9f, 0.04f, 0.0f);
  g3dVertex3f(0.9f, 0.028f, -0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, 0.028f, -0.028f);
  g3dVertex3f(0.9f, 0.0f, -0.04f);
  g3dVertex3f(0.9f, 0.028f, -0.028f);
  g3dVertex3f(0.9f, 0.0f, -0.04f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, 0.0f, -0.04f);
  g3dVertex3f(0.9f, -0.028f, -0.028f);
  g3dVertex3f(0.9f, 0.0f, -0.04f);
  g3dVertex3f(0.9f, -0.028f, -0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, -0.028f, -0.028f);
  g3dVertex3f(0.9f, -0.04f, 0.0f);
  g3dVertex3f(0.9f, -0.028f, -0.028f);
  g3dVertex3f(0.9f, -0.04f, 0.0f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, -0.04f, 0.0f);
  g3dVertex3f(0.9f, -0.028f, 0.028f);
  g3dVertex3f(0.9f, -0.04f, 0.0f);
  g3dVertex3f(0.9f, -0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  g3dVertex3f(1.0f, 0.0f, 0.0f);
  g3dVertex3f(0.9f, -0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.04f);
  g3dVertex3f(0.9f, -0.028f, 0.028f);
  g3dVertex3f(0.9f, 0.0f, 0.04f);
  g3dVertex3f(0.9f, 0.0f, 0.0f);
  // y axis arrow
  g3dColor3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.0f, 0.9f, 0.04f);
  g3dVertex3f(0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.04f);
  g3dVertex3f(0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.04f, 0.9f, 0.0f);
  g3dVertex3f(0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.04f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.04f, 0.9f, 0.0f);
  g3dVertex3f(0.028f, 0.9f, -0.028f);
  g3dVertex3f(0.04f, 0.9f, 0.0f);
  g3dVertex3f(0.028f, 0.9f, -0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.028f, 0.9f, -0.028f);
  g3dVertex3f(0.0f, 0.9f, -0.04f);
  g3dVertex3f(0.028f, 0.9f, -0.028f);
  g3dVertex3f(0.0f, 0.9f, -0.04f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(0.0f, 0.9f, -0.04f);
  g3dVertex3f(-0.028f, 0.9f, -0.028f);
  g3dVertex3f(0.0f, 0.9f, -0.04f);
  g3dVertex3f(-0.028f, 0.9f, -0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(-0.028f, 0.9f, -0.028f);
  g3dVertex3f(-0.04f, 0.9f, 0.0f);
  g3dVertex3f(-0.028f, 0.9f, -0.028f);
  g3dVertex3f(-0.04f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(-0.04f, 0.9f, 0.0f);
  g3dVertex3f(-0.028f, 0.9f, 0.028f);
  g3dVertex3f(-0.04f, 0.9f, 0.0f);
  g3dVertex3f(-0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  g3dVertex3f(0.0f, 1.0f, 0.0f);
  g3dVertex3f(-0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.04f);
  g3dVertex3f(-0.028f, 0.9f, 0.028f);
  g3dVertex3f(0.0f, 0.9f, 0.04f);
  g3dVertex3f(0.0f, 0.9f, 0.0f);
  // z axis arrow
  g3dColor3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.0f, 0.04f, 0.9f);
  g3dVertex3f(0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.04f, 0.9f);
  g3dVertex3f(0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.04f, 0.0f, 0.9f);
  g3dVertex3f(0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.04f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.04f, 0.0f, 0.9f);
  g3dVertex3f(0.028f, -0.028f, 0.9f);
  g3dVertex3f(0.04f, 0.0f, 0.9f);
  g3dVertex3f(0.028f, -0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.028f, -0.028f, 0.9f);
  g3dVertex3f(0.0f, -0.04f, 0.9f);
  g3dVertex3f(0.028f, -0.028f, 0.9f);
  g3dVertex3f(0.0f, -0.04f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(0.0f, -0.04f, 0.9f);
  g3dVertex3f(-0.028f, -0.028f, 0.9f);
  g3dVertex3f(0.0f, -0.04f, 0.9f);
  g3dVertex3f(-0.028f, -0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(-0.028f, -0.028f, 0.9f);
  g3dVertex3f(-0.04f, 0.0f, 0.9f);
  g3dVertex3f(-0.028f, -0.028f, 0.9f);
  g3dVertex3f(-0.04f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(-0.04f, 0.0f, 0.9f);
  g3dVertex3f(-0.028f, 0.028f, 0.9f);
  g3dVertex3f(-0.04f, 0.0f, 0.9f);
  g3dVertex3f(-0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 1.0f);
  g3dVertex3f(-0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.04f, 0.9f);
  g3dVertex3f(-0.028f, 0.028f, 0.9f);
  g3dVertex3f(0.0f, 0.04f, 0.9f);
  g3dVertex3f(0.0f, 0.0f, 0.9f);
  g3dEnd();
  //g3dPopMatrix();
  //g3dEnable(GL_LIGHTING);
  //g3dColor3f(1.0f, 1.0f, 0.0f);
}

