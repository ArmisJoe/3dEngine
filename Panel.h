#pragma once

class Panel {
public:
	Panel();
	virtual ~Panel();

	virtual void Draw() = 0;

public:
	bool active = true;
	int x, y, w, h;

public:
	char* name;

};