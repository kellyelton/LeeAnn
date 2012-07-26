#pragma once
#ifndef _narray_h_
#define _narray_h_
namespace Skylabs
{
	namespace Common
	{
		template<class aType>
		class nArray
		{
		public:
			aType& operator[](int i)
			{
				return Array[i];
			}
			nArray()
			{
				_Size = 0;
				_Count = 0;
			}
			nArray(int Count)
			{
				Array = new aType[(const int)(Count)+2]();
				_Size = Count+2;
				_Count = 0;
			}

			~nArray()
			{
			}
			int Resize(int newSize)
			{
				aType *temp = new aType[(const int)(newSize)+2];
				if(temp == NULL)
					return 1;
				copy(Array,Array+_Count,temp);
				Array = temp;
				_Size = newSize+2;
				return 0;
			}
			int push_back(const aType Item)
			{
				if(_Count+1 >= _Size)
				{
					if(_Size > 0)
					{
						if(Resize(_Size*2) == 1)
							return -1;
					}
					else
					{
						if(Resize(2) == 1)
							return -1;
					}
				}
				Array[_Count] = Item;
				++_Count;
				return _Count - 1;
			}
			int getSize()
			{
				return _Count;
			}
			aType * getAt(int Index)
			{
				if(Index > _Size-1 || Index >= _Count)
				{
					return NULL;
				}
				return & Array[Index];
			}
		private:
			int _Size;
			int _Count;
			aType * Array;
		};
	}
}
#endif

