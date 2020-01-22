#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 worldPosition

uniform vec2 position; //position of center of card
uniform sampler2D texture0; //emmision
//uniform sampler2D texture1; //albedo
//uniform sampler2D texture2; //normal
//uniform sampler2D texture3; //depth/paralax
//uniform sampler2D texture4; //metalicness
uniform float thickness;
uniform float scale;
//camera position = 0,0,0

out vec4 finalColor;
float stepSize = 0.1f; //make this a uniform so it can be in the settings
int stepAmmount = 10;

vec2 getTexturePosition(vec2 worldPos){
    return ((vec2(worldPos.xy-position.xy)*scale)+vec2(1,1))/2;
}

void main(){
    vec3 dir = normalize(-worldPosition);
    vec3 rayPos = dir * (thickness / dir.z);
    vec2 hitPos = getTexturePosition(lerp(worldPos, rayPos, textur(texture0)));
    //vec3 rayPos = worldPosition;
    //vec2 hitPos = worldPosition;
    
    //for(int i = 0; i < stepAmmount; i++){
    //    //we assume that our object is facing towards the camera;
    //    rayPos +=
    //    if(
    //}
    //figure out impact spot
    finalColor = vec4(hitPos.xy,0,1);
}