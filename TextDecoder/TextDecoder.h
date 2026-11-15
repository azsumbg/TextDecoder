#pragma once

#ifdef TEXTDECODER_EXPORTS
#define TEXTDECODER_API __declspec(dllexport)
#else
#define TEXTDECODER_API __declspec(dllimport)
#endif

#include <cstdlib>
#include <fstream>
#include <iterator>

constexpr int ERR_PTR = 6001;
constexpr int ERR_PARAM = 6002;
constexpr int ERR_INDEX = 6003;
constexpr int ERR_ITERATOR = 6004;
constexpr int ERR_UNKNOWN = 6005;

namespace decode
{
	class TEXTDECODER_API EXCEPTION
	{
	private:
		int err_code{ 0 };

	public:
		EXCEPTION(int what);

		const wchar_t* eGet()const;
	};

	template <typename T> class BAG
	{
	private:
		size_t max_size{ 1 };
		T* mPtr{ nullptr };
		size_t next_pos{ 0 };

	public:
		BAG() :mPtr{ reinterpret_cast<T*>(calloc(max_size, sizeof(T))) } 
		{
			if (!mPtr)throw EXCEPTION(ERR_UNKNOWN);
		};
		BAG(size_t lenght)
		{
			max_size = lenght;
			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			if (!mPtr)throw EXCEPTION(ERR_UNKNOWN);
		}
		BAG(BAG& other)
		{
			if (!other.mPtr)mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;

				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (!mPtr)throw EXCEPTION(ERR_UNKNOWN);
				else for (size_t i = 0; i < next_pos; ++i)mPtr[i] = other.mPtr[i];
			}
		}
		BAG(BAG&& other)
		{
			if (!other.mPtr)mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;
				mPtr = other.mPtr;

				other.mPtr = nullptr;
			}
		}

		~BAG()
		{
			free(mPtr);
		}

		BAG& operator= (const BAG& other)
		{
			free(mPtr);

			if (!other.mPtr)mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
			else if (mPtr == other.mPtr)return (*this);
			else
			{
				max_size = other.max_size;
				next_pos = other.next_pos;

				mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));
				if (!mPtr)throw EXCEPTION(ERR_UNKNOWN);
				else for (size_t i = 0; i < next_pos; ++i)mPtr[i] = other.mPtr[i];
			}

			return (*this);
		}
		BAG& operator= (BAG&& other)
		{
			if (mPtr == other.mPtr)return (*this);
			else
			{
				free(mPtr);

				mPtr = other.mPtr;
				max_size = other.max_size;
				next_pos = other.next_pos;

				other.mPtr = nullptr;
			}

			return(*this);
		}

		T& operator[] (size_t index)
		{
			if (index >= next_pos)throw EXCEPTION(ERR_PARAM);
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			
			return mPtr[index];
		}

		size_t size() const
		{
			return next_pos;
		}
		size_t capacity() const
		{
			return max_size;
		}
		bool empty() const
		{
			return (next_pos == 0);
		}

		void clear()
		{
			free(mPtr);

			max_size = 1;
			next_pos = 0;

			mPtr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

			if (!mPtr)throw EXCEPTION(ERR_UNKNOWN);
		}

		void push_front(T element)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t i = next_pos; i > 0; ++i)mPtr[i] = mPtr[i - 1];

					*mPtr = element;

					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };
					if (!temp)throw EXCEPTION(ERR_UNKNOWN);
					else
					{
						mPtr = temp;
						temp = nullptr;

						for (size_t i = next_pos; i > 0; ++i)mPtr[i] = mPtr[i - 1];

						*mPtr = element;

						++next_pos;
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t i = next_pos; i > 0; ++i)mPtr[i] = mPtr[i - 1];

					*mPtr = *element;

					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };
					if (!temp)throw EXCEPTION(ERR_UNKNOWN);
					else
					{
						mPtr = temp;
						temp = nullptr;

						for (size_t i = next_pos; i > 0; ++i)mPtr[i] = mPtr[i - 1];

						*mPtr = *element;

						++next_pos;
					}
				}
			}
		}

		void push_back(T element)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					mPtr[next_pos] = element;

					++next_pos;
				}
				else
				{
					++max_size;
					
					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };
					if (!temp)throw EXCEPTION(ERR_UNKNOWN);
					else
					{
						mPtr = temp;
						temp = nullptr;

						mPtr[next_pos] = element;

						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					mPtr[next_pos] = *element;

					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };
					if (!temp)throw EXCEPTION(ERR_UNKNOWN);
					else
					{
						mPtr = temp;
						temp = nullptr;

						mPtr[next_pos] = *element;

						++next_pos;
					}
				}
			}
		}

		void erase(size_t index)
		{
			if (index >= next_pos)throw EXCEPTION(ERR_INDEX);
			if (!mPtr)throw EXCEPTION(ERR_PTR);
			else
			{
				for (size_t i = index; i < next_pos - 1; ++i)mPtr[i] = mPtr[i + 1];
				
				--max_size;

				T* temp = reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T)));
				if (!temp)throw EXCEPTION(ERR_UNKNOWN);
				else mPtr = temp;
					
				--next_pos;
			}
		}

		void insert(T element, size_t index)
		{
			if (index > next_pos)throw EXCEPTION(ERR_INDEX);
			else if (index == next_pos)push_back(element);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
					mPtr[index] = element;
					++next_pos;
				}
				else
				{
					++max_size;
					
					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };

					if (!temp)throw EXCEPTION(ERR_UNKNOWN);
					else
					{
						mPtr = temp;
						for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
						mPtr[index] = element;
						++next_pos;
					}
				}
			}
		}
		void insert(T* element, size_t index)
		{
			if (index > next_pos)throw EXCEPTION(ERR_INDEX);
			else if (index == next_pos)push_back(*element);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
					mPtr[index] = *element;
					++next_pos;
				}
				else
				{
					++max_size;

					T* temp{ reinterpret_cast<T*>(realloc(mPtr, max_size * sizeof(T))) };

					if (!temp)throw EXCEPTION(ERR_UNKNOWN);
					else
					{
						mPtr = temp;
						for (size_t i = next_pos; i > index; --i)mPtr[i] = mPtr[i - 1];
						mPtr[index] = *element;
						++next_pos;
					}
				}
			}
		}

		class iterator
		{
		private:
			T* it_ptr{ nullptr };
			T* container_ptr{ nullptr };
			size_t container_size{ 0 };

		public:
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class BAG<T>;

			iterator(T* init_ptr, BAG<T>& this_bag) :it_ptr{ init_ptr }
			{
				container_ptr = this_bag.mPtr;
				container_size = this_bag.next_pos;
			}

			iterator& operator++ ()
			{
				++it_ptr;

				return(*this);
			}
			iterator operator++ (int)
			{
				T* temp{ it_ptr };
				++(*this);

				return temp;
			}

			iterator& operator-- ()
			{
				--it_ptr;

				return(*this);
			}
			iterator operator-- (int)
			{
				T* temp{ it_ptr };
				--(*this);

				return temp;
			}

			iterator& operator+ (int step)
			{
				if (it_ptr > container_ptr + container_size)throw EXCEPTION(ERR_ITERATOR);

				it_ptr += step;

				return (*this);
			}
			iterator& operator- (int step)
			{
				if (it_ptr < container_ptr)throw EXCEPTION(ERR_ITERATOR);

				it_ptr -= step;

				return(*this);
			}

			iterator& operator+= (int step)
			{
				if (it_ptr > container_ptr + container_size)throw EXCEPTION(ERR_ITERATOR);

				it_ptr += step;

				return (*this);
			}
			iterator& operator-= (int step)
			{
				if (it_ptr < container_ptr)throw EXCEPTION(ERR_ITERATOR);

				it_ptr-=step;

				return(*this);
			}

			pointer operator-> () const
			{
				return it_ptr;
			}
			reference operator* ()
			{
				return *it_ptr;
			}

			friend bool operator== (const iterator& first, const iterator& second) const
			{
				return (first.it_ptr == second.it_ptr);
			}
			friend bool operator>= (const iterator& first, const iterator& second) const
			{
				return (first.it_ptr >= second.it_ptr);
			}
			friend bool operator<= (const iterator& first, const iterator& second) const
			{
				return (first.it_ptr <= second.it_ptr);
			}
			friend bool operator> (const iterator & first, const iterator & second) const
			{
				return (first.it_ptr > second.it_ptr);
			}
			friend bool operator< (const iterator& first, const iterator& second) const
			{
				return (first.it_ptr < second.it_ptr);
			}
			friend bool operator!= (const iterator& first, const iterator& second) const
			{
				return (first.it_ptr != second.it_ptr);
			}
		};

		iterator begin()
		{
			return iterator(mPtr, (*this));
		}
		iterator end()
		{
			return(mPtr + next_pos, (*this));
		}

		void erase(iterator element)
		{
			size_t which_element = 0;
			bool found = false;

			for (size_t i = 0; i < next_pos; ++i)
			{
				if (element.it_ptr == mPtr + i)
				{
					which_element = i;
					found = true;
				}
			}

			if (found)
			{
				erase(which_element);
				return;
			}

			throw EXCEPTION(ERR_ITERATOR);
		}
	};


}