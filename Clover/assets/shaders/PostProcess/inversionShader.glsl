			#type vertex
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec2 a_TexCoord;

			out vec2 texCoord;

			void main()
			{
				gl_Position = vec4((2 * a_Position), 1.0f);
				texCoord = a_TexCoord;
			}

			#type fragment
			#version 330 core
	
			out vec4 o_color;

			in vec2 texCoord;

			uniform sampler2D u_texture; 

			
			void main()
			{
				o_color = vec4(vec3(1.0f - texture(u_texture, texCoord)), 1.0f);
			}

		