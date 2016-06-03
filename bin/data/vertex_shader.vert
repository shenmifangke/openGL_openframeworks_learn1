#version 330 core//或者#version 330 core 对应opengl版本 core为核心功能
layout (location = 0)in vec3 vp;//layout (location = 0)是位置值 方便取出 声明所有的输入顶点属性，使用in关键字 现在我们只关心位置数据（position），所以我们只需要一个顶点属性（attribute）
void main ()
{
  gl_Position = vec4 (vp, 1.0);//gl_Position = vec4(position.x, position.y, position.z, 1.0);
}