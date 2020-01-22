#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 worldPosition;

uniform vec3 model; //position of center of card
uniform sampler2D texture0; //emmision
//uniform sampler2D texture1; //albedo
//uniform sampler2D texture2; //normal
//uniform sampler2D texture3; //depth/paralax
//uniform sampler2D texture4; //metalicness
float thickness = 0.1f;
uniform float scale;
//camera position = 0,1,0

out vec4 finalColor;
int stepAmmount = 100; //make a uniform so it can be in quality settings

vec2 getTexturePosition(vec2 worldPos){
    return clamp(worldPos-model.xz+ vec2(0.5,0.5),vec2(0,0),vec2(1,1));
} 

void main(){
    thickness = thickness * scale;
    vec3 dir = normalize(vec3(0,1,0)-worldPosition);
    vec3 rayPos = worldPosition + (dir * (thickness / dir.y));
    vec2 hitPos = getTexturePosition(worldPosition.xz);
    float height = -100;
    for(int i = stepAmmount; i >= 0; i--){
        vec2 tempPos = getTexturePosition(mix(worldPosition, rayPos, float(i)/float(stepAmmount)).xz);
        if(mix(worldPosition, rayPos, float(i)/float(stepAmmount)).y/thickness <= texture(texture0, tempPos).x && !(tempPos.x >= 1.01 || tempPos.x<=-0.01 || tempPos.y<=-0.01 || tempPos.y>=1.01)){
            hitPos = tempPos;
            break;
        }
    }
    if(hitPos.x >= 1 || hitPos.x<=0 || hitPos.y<=0 || hitPos.y>=1 || (texture(texture0,hitPos).x<=0.01)){
        discard;
    }
    finalColor = vec4(texture(texture0,hitPos));
}