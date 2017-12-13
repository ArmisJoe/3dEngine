#include "DebugDraw.h"


#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

DebugDraw::DebugDraw()
{
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::SetLinesSize(float argsize)
{
	if (argsize > 0) line_size = argsize;
}

void DebugDraw::Draw(float3 * line_points, int size, float3 rgb, float a)
{
	if (active)
	{
		
		rgb /= 255;

		color.Set(rgb.x, rgb.y, rgb.z, a);

		glColor3f(color.r, color.g, color.b);

		glLineWidth((float)5);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (float*)line_points->ptr());

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0,&color );


		glDrawArrays(GL_LINES, 0, size);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glLineWidth(1);

		glColor3f(255, 255, 255);
	}
}

void DebugDraw::DrawFrustum(float3 * corners)
{
	int size = 24;

	float3* lines = new float3[size];

	lines[0] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[1] = float3(corners[2].x, corners[2].y, corners[2].z);

	lines[2] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[3] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[4] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[5] = float3(corners[6].x, corners[6].y, corners[6].z);

	lines[6] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[7] = float3(corners[0].x, corners[0].y, corners[0].z);

	//

	lines[8] = float3(corners[1].x, corners[1].y, corners[1].z);
	lines[9] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[10] = float3(corners[3].x, corners[3].y, corners[3].z);
	lines[11] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[12] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[13] = float3(corners[7].x, corners[7].y, corners[7].z);

	lines[14] = float3(corners[5].x, corners[5].y, corners[5].z);
	lines[15] = float3(corners[1].x, corners[1].y, corners[1].z);

	//

	lines[16] = float3(corners[0].x, corners[0].y, corners[0].z);
	lines[17] = float3(corners[1].x, corners[1].y, corners[1].z);

	lines[18] = float3(corners[2].x, corners[2].y, corners[2].z);
	lines[19] = float3(corners[3].x, corners[3].y, corners[3].z);

	lines[20] = float3(corners[4].x, corners[4].y, corners[4].z);
	lines[21] = float3(corners[5].x, corners[5].y, corners[5].z);

	lines[22] = float3(corners[6].x, corners[6].y, corners[6].z);
	lines[23] = float3(corners[7].x, corners[7].y, corners[7].z);

	Draw(lines, size);

	delete[] lines;

}

void DebugDraw::DrawAABB(float3 center, float3 size, float3 rgb, float a)
{
	int s = 24;

	float3* lines = new float3[s];
	float3* colors = new float3[s];

	float3 half_size = size / 2;

	lines[0] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	lines[1] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);

	lines[2] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	lines[3] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	lines[4] = float3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);
	lines[5] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[6] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);
	lines[7] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[8] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);
	lines[9] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);

	lines[10] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);
	lines[11] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);

	lines[12] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[13] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);

	lines[14] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[15] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);

	lines[16] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
	lines[17] = float3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);

	lines[18] = float3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[19] = float3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[20] = float3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
	lines[21] = float3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

	lines[22] = float3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
	lines[23] = float3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);


	Draw(lines, s, rgb, a);

	delete[] lines;
	delete[] colors;
}

void DebugDraw::Switch()
{
	if (active == false) active = true;
	else active = false;
}

void DebugDraw::SetColor(Color argcol)
{
	color = argcol;
}
