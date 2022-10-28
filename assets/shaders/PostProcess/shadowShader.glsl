		
			#type vertex
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec2 a_TexCoord;
				
			out vec2 texCoord;

			uniform mat4 u_lightSpace_matrix;	
			uniform mat4 u_model_matrix;

			void main()
			{
				texCoord = a_TexCoord;
				gl_Position = u_lightSpace_matrix * u_model_matrix * vec4(a_Position, 1.0f);
			}




			#type fragment
			#version 330 core
	
			layout (location = 0) out vec4 o_color;

			uniform sampler2D u_depth;
			in vec2 texCoord;

			void main()
			{
			    float depthValue = texture(u_depth, texCoord).r;
				o_color = vec4(vec3(depthValue), 1.0);
			}

		