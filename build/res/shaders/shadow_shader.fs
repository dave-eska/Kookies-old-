#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 lightPos;
uniform vec4 lightColor;

out vec4 finalColor;

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);

    // Calculate the distance from the current pixel to the light source
    float distance = length(gl_FragCoord.xy - lightPos);

    // Determine the light intensity based on distance
    float lightIntensity = smoothstep(50.0, 200.0, distance);  // Adjust these values to fit your needs

    // Mix the original color with the light color based on the light intensity (inverse shadow effect)
    vec4 color = mix(texColor, lightColor, 1.0 - lightIntensity);

    finalColor = color;
}
