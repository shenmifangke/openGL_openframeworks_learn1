#include "ofApp.h"
/*
http://bullteacher.com/category/zh_learnopengl_com
http://bullteacher.com/5-hello-triangle.html
http://antongerdelan.net/opengl/hellotriangle.html

http://learnopengl-cn.readthedocs.org/zh/latest/


http://www.learnopengl.com/#!Code-repository


*/

string  ofApp::loadFragmentShader( string sPath )
{
    ofFile F;
//    F.clear();
//    F.close();
    F.open(ofToDataPath(sPath), ofFile::ReadWrite, false);
    string ccc = F.readToBuffer().getText();
    return ccc;
}


GLuint EBO;//索引缓冲对象
GLuint vbo;//顶点缓冲对象
GLuint vao;//顶点数组对象
const char* vertex_shader;
const char* fragment_shader;
GLuint shader_programme;
//core-profile模式 所有新版本的OpenGL都是以3.3为基础
//这里使用的就是glfw3
void ofApp::setup(){
//float points[] = {
//     第一个三角形
//    0.5f, 0.5f, 0.0f,   // 右上角
//    0.5f, -0.5f, 0.0f,  // 右下角
//    -0.5f, 0.5f, 0.0f,  // 左上角
//
//     第二个三角形
//    0.5f, -0.5f, 0.0f,  // 右下角
//    -0.5f, -0.5f, 0.0f, // 左下角
//    -0.5f, 0.5f, 0.0f   // 左上角
//};
//我们就可以绘制两个三角形来组成一个矩形（OpenGL主要就是绘制三角形） 上面那种有浪费 所以用下面两个组合来代替
GLfloat points[] = {

    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

GLuint indices[] = { // 起始于0! 索引数组

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
// 2. 把我们的顶点数组复制到一个顶点缓冲中，提供给OpenGL使用
glGenBuffers(1, &EBO);






//----------------------------------------------------vao绑定
//VAO可以理解为一个状态容器，记录VBO的状态。VAO是OpenGL 3.0以后才引入的新东西 是一个容器，可以包括多个VBO,  它类似于以前的call list, 由于它进一步将VBO容于其中，所以绘制效率将在VBO的基础上更进一步。
//顶点数组对象（Vertex Array Object也叫做VAO）可以像顶点缓冲对象一样绑定，任何随后的顶点属性调用都会储存在这个VAO中。
//VAO的全名是Vertex Array Object，首先，它不是Buffer-Object，所以不用作存储数据；其次，它针对“顶点”而言，也就是说它跟“顶点的绘制”息息相关。（VAO和VA没有任何关系）
vao = 0;
glGenVertexArrays (1, &vao);
glBindVertexArray (vao);



//VBO是VA（Vertex Array）的升级版 VBO将顶点信息放到GPU中 VBO其实就是显卡中的显存，为了提高渲染速度，可以将要绘制的顶点数据缓存在显存中，这样就不需要将要绘制的顶点数据重复从CPU发送到GPU, 浪费带宽资源。
//顶点缓冲对象（vertex buffer objects, VBO）使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。
//GPU在渲染时去缓存中取数据，二者中间的桥梁是GL-Context。GL-Context整个程序一般只有一个，所以如果一个渲染流程里有两份不同的绘制代码，GL-context就负责在他们之间进行切换。
//这也是为什么要在渲染过程中，在每份绘制代码之中会有glBindbuffer、glEnableVertexAttribArray、glVertexAttribPointer。那么优化的方法来了，把这些都放到初始化时候完成吧！ VAO记录该次绘制所需要的所有VBO所需信息，把它保存到VBO特定位置，绘制的时候直接在这个位置取信息绘制。
vbo = 0;
glGenBuffers (1, &vbo);//这个缓冲有一个独一无二的ID 使用glGenBuffers函数生成一个缓冲ID OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
glBindBuffer (GL_ARRAY_BUFFER, vbo);//从这一刻起，我们使用的任何缓冲函数（在GL_ARRAY_BUFFER目标上）都会用来配置当前绑定的缓冲（VBO）
glBufferData (GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);//glBufferData是一个用来把用户定义数据复制到当前绑定缓冲的函数 中间也能用sizeof (points)


//ELEMENT索引缓冲对象简称EBO（或IBO）
// 3. 复制我们的索引数组到一个索引缓冲中，提供给OpenGL使用
//一个EBO是一个想顶点缓冲对象一样的缓冲，它专门储存索引，OpenGL调用这些顶点的索引来绘制。索引绘制正是这个问题的解决方案。
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);

// 3. 设置顶点属性指针
//下面也能放在绑定之前
//告诉OpenGL如何解释顶点数据了（每一个顶点属性）

//第一个参数指定我们要配置哪一个顶点属性。记住，我们在顶点着色器中使用layout(location = 0)定义了顶点属性——位置（position）的location。这样要把顶点属性的location设置为0，因为我们希望把数据传递到这个顶点属性中，所以我们在这里填0。
//下一个参数指定顶点属性的大小。顶点属性是vec3类型，它由3个数值组成
//第三个参数指定数据的类型，这里是GL_FLOAT（GLSL中vec*是由浮点数组成的）。
//第四个参数定义我们是否希望数据被标准化。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
//第五个参数叫做步长（stride），它告诉我们在连续的顶点属性之间间隔有多少。由于下个位置数据在3个GLfloat后面的位置，我们把步长设置为3 * sizeof(GLfloat)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
//最后一个参数有古怪的GLvoid*的强制类型转换。它我们的位置数据在缓冲中起始位置的偏移量。由于位置数据是数组的开始，所以这里是0
glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
glEnableVertexAttribArray (0);//定义OpenGL如何解释顶点数据，我们也要开启顶点属性



glBindVertexArray(0);//解开vao绑定 通常情况下当我们配置好它们以后要解绑OpenGL对象，这样我们才不会在某处错误地配置它们。不解绑EBO（译注：解绑缓冲相当于没有绑定缓冲，可以在解绑VAO之后解绑缓冲）
//----------------------------------------------------vao绑定

string a = loadFragmentShader("fragment.frag");

string b = loadFragmentShader("vertex_shader.vert");


fragment_shader =a.c_str();//必须用string变量进行一次多余操作 否则会有未知原因 可能是执行速度造成未载入
vertex_shader = b.c_str();//


//时动态编译glsl的源码 创建一个着色器对象，再次引用它的ID。所以我们储存这个顶点着色器为GLuint，然后用glCreateShader创建着色器
GLuint vs = glCreateShader (GL_VERTEX_SHADER);//把着色器的类型提供glCreateShader作为它的参数。这里我们传递的参数是GL_VERTEX_SHADER这样就创建了一个顶点着色器
glShaderSource (vs, 1, &vertex_shader, NULL);//把这个着色器源码附加到着色器对象，然后编译它 第二参数指定了源码中有多少个字符串，这里只有一个。第三个参数是顶点着色器真正的源码
glCompileShader (vs);//这步之后才能进行错误检测
//vs错误检测
GLint success;
GLchar infoLog[512];//储存错误消息的容器
glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}



GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);//使用GL_FRAGMENT_SHADER作为着色器类型
glShaderSource (fs, 1, &fragment_shader, NULL);
glCompileShader (fs);
//fs错误检测
GLint success2;
GLchar infoLog2[512];
glGetShaderiv(fs, GL_COMPILE_STATUS, &success2);
if(!success2)
{
    glGetShaderInfoLog(fs, 512, NULL, infoLog2);
    std::cout << "ERROR::SHADER::FRANMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
}


//着色器程序对象（shader program object）是多个着色器最后链接的版本。如果要使用刚才编译的着色器我们必须把它们链接为一个着色器程序对象，然后当渲染物体的时候激活这个着色器程序。激活了的着色器程序的着色器，在调用渲染函数时才可用。
//把着色器链接为一个程序就等于把每个着色器的输出链接到下一个着色器的输入。如果你的输出和输入不匹配那么就会得到一个链接错误
shader_programme = glCreateProgram ();
glAttachShader (shader_programme, fs);
glAttachShader (shader_programme, vs);
glLinkProgram (shader_programme);
//glCreateProgram函数创建一个程序，返回新创建的程序对象的ID引用。现在我们需要把前面编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
//着色器程序对象附加错误检测
GLint success3;
GLchar infoLog3[512];
glGetProgramiv(shader_programme, GL_LINK_STATUS, &success3);

if(!success3) {
    glGetProgramInfoLog(shader_programme, 512, NULL, infoLog3);
}


//在我们把着色器对象链接到程序对象以后，不要忘记删除着色器对象；我们不再需要它们了：
glDeleteShader(vs);
glDeleteShader(fs);




}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//必须放在draw里 glClearColor函数是一个状态设置函数
glClear(GL_COLOR_BUFFER_BIT);//glClear是一个状态使用函数

  // wipe the drawing surface clear
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (shader_programme);//调用glUseProgram函数，用新创建的程序对象作为它的参数，这样就能激活这个程序对象
  glBindVertexArray (vao);
  // draw points 0-3 from the currently bound VAO with current in-use shader
  //第一个参数是我们打算绘制的OpenGL基本图形的类型
  //第二个参数定义了我们打算绘制的那个顶点数组的起始位置的索引；我们这里填0
  //最后一个参数指定我们打算绘制多少个顶点，这里是3（我们只从我们的数据渲染一个三角形，它只有3个顶点）。
//  glDrawArrays (GL_TRIANGLES, 0, 3);

//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//用GL_ELEMENT_ARRAY_BUFFER当作缓冲目标  不同于上面一种绘制方式
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao);//用GL_ELEMENT_ARRAY_BUFFER当作缓冲目标  不同于上面一种绘制方式
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) 线模式

  //第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样
  //第二个参数是我们打算绘制顶点的次数。我们填6，说明我们总共想绘制6个顶点。
  //第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
  //最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这只是当你不是在使用索引缓冲对象的时候），但是我们只打算在这里填写0
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//用glDrawElements来替换glDrawArray 来指明我们从索引缓冲渲染

  glBindVertexArray(0);//解开绑定
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
