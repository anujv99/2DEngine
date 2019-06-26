#pragma once

class gmMachine;
class gmTableObject;
struct gmVariable;

bool gmFileExists(const char * file);

int gmCompileStr(gmMachine * a_machine, const char * file);
