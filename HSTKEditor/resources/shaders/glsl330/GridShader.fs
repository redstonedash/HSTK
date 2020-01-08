#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform vec2 offset;
uniform ivec2 resolution;
uniform int spacing;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Texel color fetching from texture sampler
    vec2 CenteredTexCoord = ((fragTexCoord) * resolution) - (resolution/2) + offset;
    vec2 gridSecCoord = mod(CenteredTexCoord,vec2(spacing*10,spacing*10));
    float gridBrightnessAtCoord = (step(gridSecCoord.x,1) + step(gridSecCoord.y,1))/2;
    gridSecCoord = mod(CenteredTexCoord,vec2(spacing,spacing));
    gridBrightnessAtCoord += (step(gridSecCoord.x,1) + step(gridSecCoord.y,1))/2;
    vec4 texelColor = vec4(gridBrightnessAtCoord, gridBrightnessAtCoord, gridBrightnessAtCoord, 1.0f);

    if(step(abs(CenteredTexCoord.x),1)!=0) {
    	texelColor = vec4(1, 0, 0, 1.0f);
    }

    if(step(abs(CenteredTexCoord.y),1)!=0) {
    	texelColor = vec4(0, 1, 0, 1.0f);
    }

    finalColor = texelColor;
}

