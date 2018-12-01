#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("This file demonstrates a simple double-free attack with fastbins.\n");

	printf("Allocating 3 buffers.\n");
	int *a = malloc(8);
	int *b = malloc(8);
	int *c = malloc(8);

	printf("1st malloc(8): %p\n", a);
	printf("2nd malloc(8): %p\n", b);
	printf("3rd malloc(8): %p\n", c);

	printf("Freeing the first one...\n");
	free(a);

	printf("If we free %p again, things will crash because %p is at the top of the free list.\n", a, a);
	// free(a);   不能直接释放a，应为a刚释放过，在fastbins链表头部，释放会报错

	printf("So, instead, we'll free %p.\n", b);
	free(b);   //释放b，将b，会将b加入到fastbins头部，后面紧跟的就是a

	printf("Now, we can free %p again, since it's not the head of the free list.\n", a);
	free(a);   //fastbin表，如果不进行大数据操作，让fastbins表清空，不会释放，再次释放，会导致链表头尾互相指向

	printf("Now the free list has [ %p, %p, %p ]. If we malloc 3 times, we'll get %p twice!\n", a, b, a, a);
	printf("1st malloc(8): %p\n", malloc(8));
	printf("2nd malloc(8): %p\n", malloc(8));
	printf("3rd malloc(8): %p\n", malloc(8));
}
