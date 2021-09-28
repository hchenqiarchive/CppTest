
int current_index = 0;

int AllocateIndex(){
	return current_index ++;
}

int AllocateIndex(int size){
	return current_index += size;  // error! returns the modified value!
}