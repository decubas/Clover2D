		
			#type vertex
			#version 450 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec2 a_TexCoord;
			layout (location = 2) in vec3 a_Normal;
			//layout (location = 3) in vec4 a_Color;
			
			uniform int u_ObjectID;
			uniform vec4 u_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
		
			out vec3 pos;
			out vec2 texCoord;
			out vec3 normal;
			out vec4 color;
			out flat int v_ObjectID;
			
			void main()
			{
				pos = a_Position;
				texCoord = a_TexCoord;
				normal = a_Normal;
				color = u_Color;
				v_ObjectID = u_ObjectID;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}




			#type fragment
			#version 450 core
	
			layout (location = 0) out vec4 o_color;
			layout (location = 1) out int o_IDBuffer;


		//USER
			uniform sampler2D u_Diffuse; // diffuse


			in vec3 pos;
			in vec2 texCoord;
			in vec3 normal;
			in vec4 color;
			in flat int v_ObjectID;

			void main()
			{
			//Correccion de Gamma
				o_color = texture(u_Diffuse, texCoord) * color;
				float gamma = 2.2;
				o_color.rgb = pow(o_color.rgb, vec3(1.0/gamma));
				o_IDBuffer = v_ObjectID;
			}
