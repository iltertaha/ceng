#version 330

layout(location = 0) in vec3 position;

// Data from CPU 
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


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{

    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
    textureCoordinate = vec2(1-(position.x-slide)/widthTexture, 1-position.z/heightTexture);
    // textureCoordinate = vec2(1-position.x/widthTexture, 1-position.z/heightTexture);
    int numOfTri = 0;
    vec3 normal = vec3(0.0f, 0.0f, 0.0f);
    
    // Special coords
    vec3 base = vec3(position.x, texture2D(heightImage, vec2(1 -(position.x)/widthTexture, 1 -(position.z)/heightTexture)).x * heightFactor, position.z);
    
    if(position.x == 0 && position.z == 0){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else if (position.x == 0 && position.z == heightTexture){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else if (position.x == widthTexture && position.z == heightTexture){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else if (position.x == widthTexture && position.z == 0){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else if (position.x == 0){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else if (position.x == widthTexture){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else if (position.z == 0){
        vec3 v0 = vec3(position.x-1, texture2D(heightImage, vec2(1 -(position.x-1-slide)/widthTexture, 1 -(position.z)/heightTexture)).x * heightFactor, position.z);
        vec3 v1 = vec3(position.x, texture2D(heightImage, vec2(1 -(position.x-slide)/widthTexture, 1 -(position.z+1)/heightTexture)).x * heightFactor, position.z+1);
        vec3 v2 = vec3(position.x+1, texture2D(heightImage, vec2(1 -(position.x+1-slide)/widthTexture, 1 -(position.z+1)/heightTexture)).x * heightFactor, position.z+1);
        vec3 v3 = vec3(position.x+1, texture2D(heightImage, vec2(1 -(position.x+1-slide)/widthTexture, 1 -(position.z)/heightTexture)).x * heightFactor, position.z);
        vec3 n0 = cross(normalize(v0-base), normalize(v1-base));
        vec3 n1 = cross(normalize(v1-base), normalize(v2-base));
        vec3 n2 = cross(normalize(v2-base), normalize(v3-base));
        normal = (n0+n1+n2) / 3;
        normal = normalize(normal);
    } else if (position.z == heightTexture){
        normal = vec3(0.0f, 0.0f, 0.0f);
    } else {   
        vec3 v0 = vec3(position.x-1, texture2D(heightImage, vec2(1 -(position.x-1-slide)/widthTexture, 1 -(position.z)/heightTexture)).x * heightFactor, position.z);
        vec3 v1 = vec3(position.x, texture2D(heightImage, vec2(1 -(position.x-slide)/widthTexture, 1 -(position.z+1)/heightTexture)).x * heightFactor, position.z+1);
        vec3 v2 = vec3(position.x+1, texture2D(heightImage, vec2(1 -(position.x+1-slide)/widthTexture, 1 -(position.z+1)/heightTexture)).x * heightFactor, position.z+1);
        vec3 v3 = vec3(position.x+1, texture2D(heightImage, vec2(1 -(position.x+1-slide)/widthTexture, 1 -(position.z)/heightTexture)).x * heightFactor, position.z);
        vec3 v4 = vec3(position.x, texture2D(heightImage, vec2(1 -(position.x-slide)/widthTexture, 1 -(position.z-1)/heightTexture)).x * heightFactor, position.z-1);
        vec3 v5 = vec3(position.x-1, texture2D(heightImage, vec2(1 -(position.x-1-slide)/widthTexture, 1-(position.z-1)/heightTexture)).x * heightFactor, position.z-1);

        vec3 n0 = cross(normalize(v0-base), normalize(v1-base));
        vec3 n1 = cross(normalize(v1-base), normalize(v2-base));
        vec3 n2 = cross(normalize(v2-base), normalize(v3-base));
        vec3 n3 = cross(normalize(v3-base), normalize(v4-base));
        vec3 n4 = cross(normalize(v4-base), normalize(v5-base));
        vec3 n5 = cross(normalize(v5-base), normalize(v0-base));
        
        normal = (n0+n1+n2+n3+n4+n5) / 6;
        normal = normalize(normal);
    }

    vertexNormal = normal;
    // vertexNormal = vec3(hOfBase.x-hOfLeft.x, hOfLeft.x-hOfBase.x, 0.0f);
   // set gl_Position variable correctly to give the transformed vertex position
   vec4 height = texture2D(heightImage, textureCoordinate);
   gl_Position = MVP * vec4(position.x, height.x*heightFactor, position.z, 1.0); // this is a placeholder. It does not correctly set the position 
//    gl_Position = MVP * vec4(position.x+slide, height.x*heightFactor, position.z, 1.0); // this is a placeholder. It does not correctly set the position 
    // ToCameraVector = normalize(vec3(gl_Position.x, -gl_Position.y, -gl_Position.z));
    ToCameraVector = normalize(cameraPosition - vec4(position, 1.0)).xyz;
   ToLightVector = normalize((lightPos - vec4(position.x, height.x*heightFactor, position.z, 1.0f)).xyz);
   
}
