#version 330

// Output Color
out vec4 color;
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;
uniform int slide;

// Texture-related data
uniform sampler2D rgbTexture;
uniform sampler2D heightImage;
uniform int widthTexture;
uniform int heightTexture;
uniform vec4 lightPos;
uniform vec4 intensity;

// Data from Vertex Shader
in vec2 textureCoordinate;
in vec3 vertexNormal; // For Lighting computation
in vec3 ToLightVector; // Vector from Vertex to Light;
in vec3 ToCameraVector; // Vector from Vertex to Camera;

void main() {

  // Assignment Constants below
  // get the texture color
  
  vec4 textureColor = texture(rgbTexture, textureCoordinate);

  // apply Phong shading by using the following parameters
  vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
  vec4 Ia = vec4(0.3,0.3,0.3,1.0); // light color for ambient
  vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
  vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse
  vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular
  vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular
  int specExp = 100; // specular exponent

  // compute ambient component
  vec4 ambient = ka * Ia;
  // ambient = vec4(0.0, 0.0, 0.0, 0.0); // light color for diffuse

  // compute diffuse component
  vec4 diffuse = kd * Id * intensity * max(0, dot(ToLightVector, vertexNormal));
  // diffuse = vec4(0.0, 0.0, 0.0, 0.0);
  
  // compute specular component
  vec3 halfVec = (ToCameraVector + ToLightVector);
  float len = length(halfVec);
  halfVec = halfVec/len;
  vec4 specular = ks * Is * intensity * pow(max(0, dot(vertexNormal, halfVec)), specExp);
  // specular = vec4(0, 0,0,0);
  
  // compute the color using the following equation
  color = vec4(clamp( textureColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0), 1.0);
  // color = diffuse;
  // color = texture2D(rgbTexture, textureCoordinate);
  // color = vec4(vertexNormal.x,0,vertexNormal.z, 1);
  // color = vec4((ToCameraVector + ToLightVector).x, 0, 0, 1);
  // color = vec4(halfVec.x, 0, 0, 1);
}