#include "ResourceBone.h"
#include "mmgr\mmgr.h"

void ResourceBone::CleanUp()
{
	if (indices != nullptr)
		mdelete[] indices;
	if (weigths != nullptr)
		mdelete[] weigths;
}
