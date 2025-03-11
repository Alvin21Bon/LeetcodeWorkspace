int function(void* test)
{
	return 5;
}

int main()
{
	int (*myFunction)(void*) = function;
}
