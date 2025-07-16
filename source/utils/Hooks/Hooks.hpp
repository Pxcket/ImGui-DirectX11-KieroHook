#pragma once

// Make More 


void InitHooks()
{

	//if Using Minhook use one of these as a template 


	/*
	
	MH_CreateHook(reinterpret_cast<void**>(ModuleGA + (0x0000)), &your_FunctionName_Hook, (void**)&your_FunctionName_Original);
	MH_EnableHook(reinterpret_cast<void**>(ModuleGA + (0x0000)));
	
	*/

	/*
	uintptr_t FunctionOffset = 0x0000 //Declare this over the Init Hooks Void
	
	MH_CreateHook(reinterpret_cast<void**>(ModuleGA + (FunctionOffset)), &your_FunctionName_Hook, (void**)&your_FunctionName_Original);
	MH_EnableHook(reinterpret_cast<void**>(ModuleGA + (FunctionOffset)));
	
	*/
}