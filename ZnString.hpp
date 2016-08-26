#pragma once
#ifndef __ZNSTRING_HPP_2016_08_22
#define __ZNSTRING_HPP_2016_08_22

#pragma warning (push)
#pragma warning (disable : 4996)

namespace ZnString
{
	class _base_zn_string_allocate
	{
	public:
		typedef	unsigned char	element_t;
		element_t*				allocate(size_t _size)			{ return new element_t[_size]; }
		int						deallocate(element_t* _ptr)		{ delete[] _ptr; return 0; }
	};

	template<typename t_char>	struct _convert_type			{typedef	char		type_t;};
	template<>					struct _convert_type<char>		{typedef	wchar_t		type_t;};
	template<typename t_char>	struct _make_empty_str			{const		wchar_t *	operator ()(){return L"";};};
	template<>					struct _make_empty_str<char>	{const		char *		operator ()(){return "";};};

	template<typename t_element>
	struct _base_zn_string_ref_count_element
	{
		typedef t_element										element_t;
		size_t													count;
		element_t*												ptr;
		size_t													str_size;
		size_t													mem_size;
		typename _convert_type<element_t>::type_t*				convert_ptr;
		size_t													convert_str_size;
		size_t													convert_mem_size;
	};

	template<typename t_element, typename t_allocate>
	class _base_zn_string
	{
	public:
		typedef t_element										element_t;
		typedef typename _convert_type<element_t>::type_t		convert_element_t;
		typedef t_allocate										allocate_t;
		typedef typename allocate_t::element_t					allocate_element_t;
		typedef _base_zn_string_ref_count_element<element_t>	ref_count_element_t;
		typedef _base_zn_string<element_t, allocate_t>			self_t;
		typedef _base_zn_string<convert_element_t, allocate_t>	convert_self_t;
	public:
		_base_zn_string()
			: element_ptr_(nullptr)
		{}
		~_base_zn_string() 
		{ 
			__release_string(); 
		}
		_base_zn_string(const element_t* _value_ptr) 
			: element_ptr_(nullptr) 
		{ 
			assign(_value_ptr); 
		}
		_base_zn_string(const element_t* _value_ptr, const size_t _size) 
			: element_ptr_(nullptr) 
		{ 
			assign(_value_ptr, _size); 
		}
		_base_zn_string(const convert_element_t* _value_ptr) 
			: element_ptr_(nullptr) 
		{ 
			assign(_value_ptr); 
		}
		_base_zn_string(const convert_element_t* _value_ptr, const size_t _size) 
			: element_ptr_(nullptr) 
		{ 
			assign(_value_ptr, _size); 
		}
		_base_zn_string(const self_t& _string) 
			: element_ptr_(nullptr) 
		{ 
			__quote_string(_string.element_ptr_); 
		}
		_base_zn_string(const convert_self_t& _string) 
			: element_ptr_(nullptr) 
		{ 
			assign(_string.c_str(), _string.size()); 
		}
		const element_t*			c_str() const 
		{ 
			if (!element_ptr_) 
				return nullptr; 
			return element_ptr_->ptr; 
		}
		const element_t*			c_str_no_nullptr() const
		{
			const element_t* ptr = c_str();
			if (!ptr)
				return _make_empty_str<element_t>()();
			return ptr;
		}
		size_t						size() const 
		{ 
			if (!element_ptr_) 
				return 0; 
			return element_ptr_->str_size; 
		}
		size_t						mem_size() const
		{
			if (!element_ptr_) 
				return 0; 
			return element_ptr_->mem_size; 
		}
		bool						empty() const
		{
			if (!element_ptr_)
				return true;
			return element_ptr_->str_size == 0;
		}
		const convert_element_t*	convert_str() const
		{
			if (!element_ptr_)
				return nullptr;
			if (element_ptr_->convert_ptr)
				return element_ptr_->convert_ptr;
			_base_zn_string<element_t, allocate_t>* this_ptr = (_base_zn_string<element_t, allocate_t>*)this;
			this_ptr->__str_to_convert_str(this_ptr->element_ptr_);
			return element_ptr_->convert_ptr;
		}
		const convert_element_t*	convert_str_no_nullptr() const
		{
			const convert_element_t* ptr = convert_str();
			if (!ptr)
				return _make_empty_str<convert_element_t>()();
			return ptr;
		}
		size_t						convert_size() const
		{
			if (!element_ptr_)
				return 0;
			if (element_ptr_->convert_ptr)
				return element_ptr_->convert_str_size;
			_base_zn_string<element_t, allocate_t>* this_ptr = (_base_zn_string<element_t, allocate_t>*)this;
			this_ptr->__str_to_convert_str(this_ptr->element_ptr_);
			return element_ptr_->convert_str_size;
		}
		size_t						convert_mem_size() const
		{
			if (!element_ptr_)
				return 0;
			if (element_ptr_->convert_ptr)
				return element_ptr_->convert_mem_size;
			_base_zn_string<element_t, allocate_t>* this_ptr = (_base_zn_string<element_t, allocate_t>*)this;
			this_ptr->__str_to_convert_str(this_ptr->element_ptr_);
			return element_ptr_->convert_mem_size;
		}
		void						clear(){ __release_string(); }
		void						remove(size_t _index, size_t _size){ replace(_index, _size, (element_t*) nullptr, 0); }
		void						assign(const element_t* _value_ptr, const size_t _size){ __assign(_value_ptr, _size); }
		void						assign(const element_t* _value_ptr){ __assign(_value_ptr); }
		void						assign(const convert_element_t* _value_ptr, const size_t _size){ __assign(_value_ptr, _size); }
		void						assign(const convert_element_t* _value_ptr){ __assign(_value_ptr); }
		void						append(const element_t* _value_ptr, const size_t _size){ __append(_value_ptr, _size); }
		void						append(const element_t* _value_ptr){ __append(_value_ptr); }
		void						append(const convert_element_t* _value_ptr, const size_t _size){ __append(_value_ptr, _size); }
		void						append(const convert_element_t* _value_ptr){ __append(_value_ptr); }
		void						insert(const size_t _index, const element_t* _value_ptr, const size_t _size){ __insert(_index, _value_ptr, _size); }
		void						insert(const size_t _index, const element_t* _value_ptr){ __insert(_index, _value_ptr); }
		void						insert(const size_t _index, const convert_element_t* _value_ptr, const size_t _size){ __insert(_index, _value_ptr, _size); }
		void						insert(const size_t _index, const convert_element_t* _value_ptr){ __insert(_index, _value_ptr); }
		int 						find(const size_t _index, const element_t* _value_ptr, const size_t _size){ return __find(_index, _value_ptr, _size); }
		int 						find(const size_t _index, const element_t* _value_ptr){ return __find(_index, _value_ptr, __strlen(_value_ptr)); }
		int 						find(const size_t _index, const convert_element_t* _value_ptr, const size_t _size){ return __find(_index, _value_ptr, _size); }
		int 						find(const size_t _index, const convert_element_t* _value_ptr){ return __find(_index, _value_ptr, __strlen(_value_ptr)); }
		int 						find_and_replace(const size_t _index, const element_t* _src_ptr, const size_t _src_size, const element_t* _dst_ptr, const size_t _dst_size){ return __find_and_replace(_index, _src_ptr, _src_size, _dst_ptr, _dst_size); }
		int 						find_and_replace(const size_t _index, const element_t* _src_ptr, const element_t* _dst_ptr){ return __find_and_replace(_index, _src_ptr, __strlen(_src_ptr), _dst_ptr, __strlen(_dst_ptr)); }
		int 						find_and_replace(const size_t _index, const convert_element_t* _src_ptr, const size_t _src_size, const convert_element_t* _dst_ptr, const size_t _dst_size){ return __find_and_replace(_index, _src_ptr, _src_size, _dst_ptr, _dst_size); }
		int 						find_and_replace(const size_t _index, const convert_element_t* _src_ptr, const convert_element_t* _dst_ptr){ return __find_and_replace(_index, _src_ptr, __strlen(_src_ptr), _dst_ptr, __strlen(_dst_ptr)); }
		int 						replace(const size_t _index, const size_t _size, const element_t* _dst_ptr, const size_t _dst_size){ return __replace(_index, _size, _dst_ptr, _dst_size); }
		int 						replace(const size_t _index, const size_t _size, const element_t* _dst_ptr){ return __replace(_index, _size, _dst_ptr, __strlen(_dst_ptr)); }
		int 						replace(const size_t _index, const size_t _size, const convert_element_t* _dst_ptr, const size_t _dst_size){ return __replace(_index, _size, _dst_ptr, _dst_size); }
		int 						replace(const size_t _index, const size_t _size, const convert_element_t* _dst_ptr){ return __replace(_index, _size, _dst_ptr, __strlen(_dst_ptr)); }
		int 						compare(const element_t* _value_ptr, const size_t _size)
		{ 
			if (empty() && (!_value_ptr || _size == 0))
				return 0;
			else if (empty() && _value_ptr && _size)
				return -1;
			else if (!empty() && (!_value_ptr || _size == 0))
				return 1;
			if (size() == _size)
				return __strcmp(c_str(), _value_ptr, _size); 
			else if (size() < _size)
				return -1;
			return 1;
		}
		int 						compare(const element_t* _value_ptr){ return compare(_value_ptr, __strlen(_value_ptr)); }
		int 						compare(const convert_element_t* _value_ptr, const size_t _size)
		{ 
			if (empty() && (!_value_ptr || _size == 0))
				return 0;
			else if (empty() && _value_ptr && _size)
				return -1;
			else if (!empty() && (!_value_ptr || _size == 0))
				return 1;
			if (size() == _size)
				return __strcmp(convert_str(), _value_ptr, _size); 
			else if (size() < _size)
				return -1;
			return 1;
		}
		int 						compare(const convert_element_t* _value_ptr){ return compare(_value_ptr, __strlen(_value_ptr)); }
		int 						compare(const self_t& _string){ compare(_string.c_str(), _string.size()); }
		int 						compare(const convert_self_t& _string){ compare(_string.convert_str(), _string.convert_size()); }
		self_t						left(const size_t _size) const
		{ 
			return mid(0, _size); 
		}
		self_t						right(const size_t _size) const
		{
			size_t begin = 0;
			if (_size < size())
				begin = size() - _size;
			return mid(begin, _size);
		}
		self_t						mid(const size_t _begin, const size_t _size) const
		{
			if (_begin >= size())
				return _base_zn_string();
			size_t real_size = _size;
			if (_begin + _size > size())
				real_size = size() - _begin;
			return _base_zn_string(c_str() + _begin, real_size);
		}
		int							format(size_t max_size, const element_t* _format_ptr, ...)
		{
			element_t* ptr = (element_t*)__new((max_size + 1) * sizeof(element_t));
			va_list arg_list;
			va_start(arg_list, _format_ptr);
			int ret = __fomat_print_s(ptr, max_size + 1, _format_ptr, arg_list);
			va_end(arg_list);
			if (ret > 0)
				assign(ptr, (size_t)ret);
			__delete(ptr);
			return ret;
		}
		int							format(const element_t* _format_ptr, ...)
		{
			va_list arg_list;
			va_start(arg_list, _format_ptr);
			int str_len = __fomat_print(nullptr, 0, _format_ptr, arg_list);
			element_t* ptr = (element_t*)__new(sizeof(element_t) * (str_len + 1));
			int ret = __fomat_print(ptr, str_len, _format_ptr, arg_list);
			va_end(arg_list);
			if (ret > 0)
				assign(ptr, (size_t)ret);
			__delete(ptr);
			return ret;
		}
	public:
		self_t&						operator= (const element_t* _value_ptr){ assign(_value_ptr, __strlen(_value_ptr)); return *this; }
		self_t&						operator= (const convert_element_t* _value_ptr){ assign(_value_ptr, __strlen(_value_ptr)); return *this; }
		self_t&						operator= (const self_t& _string){ __quote_string(_string.element_ptr_); return *this; }
		self_t&						operator= (const convert_self_t& _string){ assign(_string.c_str(), _string.size()); return *this; }
		self_t						operator+ (const element_t* _value_ptr)
		{
			_base_zn_string string(*this);
			string.append(_value_ptr);
			return string;
		}
		self_t						operator+ (const convert_element_t* _value_ptr)
		{
			_base_zn_string string(*this);
			string.append(_value_ptr);
			return string;
		}
		self_t						operator+ (const self_t& _string){ return operator+ (_string.c_str()); }
		self_t						operator+ (const convert_self_t& _string){ return operator+ (_string.c_str()); }
		self_t&						operator+=(const element_t* _value_ptr){ append(_value_ptr); return *this; }
		self_t&						operator+=(const convert_element_t* _value_ptr){ append(_value_ptr); return *this; }
		self_t&						operator+=(const self_t& _string){ append(_string.c_str()); return *this; }
		self_t&						operator+=(const convert_self_t& _string){ append(_string.c_str()); return *this;}
		bool						operator> (const element_t* _value_ptr){ return compare(_value_ptr) > 0;}
		bool						operator> (const convert_element_t* _value_ptr){ return compare(_value_ptr) > 0;}
		bool						operator> (const self_t& _string){ return compare(_string) > 0;}
		bool						operator> (const convert_self_t& _string){ return compare(_string) > 0;}
		bool						operator>=(const element_t* _value_ptr){ return compare(_value_ptr) >= 0;}
		bool						operator>=(const convert_element_t* _value_ptr){ return compare(_value_ptr) >= 0;}
		bool						operator>=(const self_t& _string){ return compare(_string) >= 0;}
		bool						operator>=(const convert_self_t& _string){ return compare(_string) >= 0;}
		bool						operator< (const element_t* _value_ptr){ return compare(_value_ptr) < 0;}
		bool						operator< (const convert_element_t* _value_ptr){ return compare(_value_ptr) < 0;}
		bool						operator< (const self_t& _string){ return compare(_string) < 0;}
		bool						operator< (const convert_self_t& _string){ return compare(_string) < 0;}
		bool						operator<=(const element_t* _value_ptr){ return compare(_value_ptr) <= 0;}
		bool						operator<=(const convert_element_t* _value_ptr){ return compare(_value_ptr) <= 0;}
		bool						operator<=(const self_t& _string){ return compare(_string) <= 0;}
		bool						operator<=(const convert_self_t& _string){ return compare(_string) <= 0;}
		bool						operator==(const element_t* _value_ptr){ return compare(_value_ptr) == 0;}
		bool						operator==(const convert_element_t* _value_ptr){ return compare(_value_ptr) == 0;}
		bool						operator==(const self_t& _string){ return compare(_string) == 0;}
		bool						operator==(const convert_self_t& _string){ return compare(_string) == 0;}
		bool						operator!=(const element_t* _value_ptr){ return compare(_value_ptr) != 0;}
		bool						operator!=(const convert_element_t* _value_ptr){ return compare(_value_ptr) != 0;}
		bool						operator!=(const self_t& _string){ return compare(_string) != 0;}
		bool						operator!=(const convert_self_t& _string){ return compare(_string) != 0;}
	private:
		void*						__new(const size_t _size){ return allocate_.allocate(_size); }
		int							__delete(const void* _ptr){ return allocate_.deallocate((allocate_element_t*)_ptr); }
		void						__new_element(ref_count_element_t** _element_ptr_ptr)
		{
			*_element_ptr_ptr = (ref_count_element_t*) __new(sizeof(ref_count_element_t));
			(*_element_ptr_ptr)->count = 1;
			(*_element_ptr_ptr)->ptr = nullptr;
			(*_element_ptr_ptr)->str_size = 0;
			(*_element_ptr_ptr)->mem_size = 0;
			(*_element_ptr_ptr)->convert_ptr = nullptr;
			(*_element_ptr_ptr)->convert_str_size = 0;
			(*_element_ptr_ptr)->convert_mem_size = 0;
		}
		void						__new_element_str(ref_count_element_t* _element_ptr, const size_t _size, const element_t* _str_ptr)
		{
			size_t str_byte = sizeof(element_t) * _size;
			size_t mem_size = __cal_mem_size(16, str_byte + sizeof(element_t));
			_element_ptr->ptr = (element_t*) __new(mem_size);
			_element_ptr->str_size = _size;
			_element_ptr->mem_size = mem_size;
			__copy_str(_element_ptr->ptr, mem_size, _str_ptr, str_byte, _size);
		}
		void						__new_element_str(ref_count_element_t* _element_ptr, const size_t _size, const convert_element_t* _convert_element_ptr)
		{
			__new_element_convert_str(_element_ptr, _size, _convert_element_ptr);
			size_t mem_size = __convert(_convert_element_ptr, _size, &(_element_ptr->ptr));
			_element_ptr->str_size = _size;
			_element_ptr->mem_size = mem_size;
		}
		void						__new_element_convert_str(ref_count_element_t* _element_ptr, const size_t _size, const convert_element_t* _convert_str_ptr)
		{
			size_t str_byte = sizeof(convert_element_t) * _size;
			size_t mem_size = __cal_mem_size(16, str_byte + sizeof(convert_element_t));
			_element_ptr->convert_ptr = (convert_element_t*) __new(mem_size);
			_element_ptr->convert_str_size = _size;
			_element_ptr->convert_mem_size = mem_size;
			__copy_str(_element_ptr->convert_ptr, mem_size, _convert_str_ptr, str_byte, _size);
		}
		void						__reset_ref_count(ref_count_element_t* _element_ptr){ _element_ptr->count = 0; }
		size_t						__add_ref_count(ref_count_element_t* _element_ptr){ return ++(_element_ptr->count); }
		size_t						__sub_ref_count(ref_count_element_t* _element_ptr){ return --(_element_ptr->count); }
		void						__quote_string(const ref_count_element_t* _ptr)
		{
			__release_string();
			element_ptr_ = (ref_count_element_t*)_ptr;
			if (element_ptr_)
				__add_ref_count(element_ptr_);
		}
		void						__release_string()
		{
			if (!element_ptr_) return;
			if (__sub_ref_count(element_ptr_) == 0)
			{
				if (element_ptr_->ptr)
					allocate_.deallocate((allocate_element_t*)element_ptr_->ptr);
				__clear_convert_str(element_ptr_);
				allocate_.deallocate((allocate_element_t*)element_ptr_);
			}
			element_ptr_ = nullptr;
		}
		size_t						__convert(const char* _char_ptr, const size_t _size, wchar_t** _wchar_ptr_ptr)
		{
			size_t byte_size = (size_t)::MultiByteToWideChar(CP_ACP, 0, _char_ptr, _size, nullptr, 0);
			size_t mem_size = __cal_mem_size(16, sizeof(wchar_t) * (byte_size + 1));
			*_wchar_ptr_ptr = (wchar_t*)__new(mem_size);
			::MultiByteToWideChar(CP_ACP, 0, _char_ptr, _size, *_wchar_ptr_ptr, byte_size);
			(*_wchar_ptr_ptr)[_size] = 0;
			return mem_size;
		}
		size_t						__convert(const wchar_t* _wchar_ptr, const size_t _size, char** _char_ptr_ptr)
		{
			size_t byte_size = (size_t)::WideCharToMultiByte(0, 0, _wchar_ptr, _size, nullptr, 0, nullptr, nullptr);
			size_t mem_size = __cal_mem_size(16, sizeof(char) * (byte_size + 1));
			*_char_ptr_ptr = (char*)__new(mem_size);
			::WideCharToMultiByte(0, 0, _wchar_ptr, _size, *_char_ptr_ptr, byte_size, nullptr, nullptr);
			(*_char_ptr_ptr)[_size] = 0;
			return mem_size;
		}
		size_t						__strlen(const char* _char_ptr) const { return strlen(_char_ptr); }
		size_t						__strlen(const wchar_t* _wchar_ptr) const { return wcslen(_wchar_ptr); }
		int							__strcmp(const char* _left, const char* _right, size_t _size) const { return strncmp(_left, _right, _size); }
		int							__strcmp(const wchar_t* _left, const wchar_t* _right, size_t _size) const { return wcsncmp(_left, _right, _size); }
		size_t						__cal_mem_size(const size_t _begin, const size_t _end) const
		{
			size_t size = _begin > 16 ? _begin : 16;
			while (size <= _end)
				size *= 2;
			return size;
		}
		void						__str_to_convert_str(ref_count_element_t* _element_ptr)
		{
			_element_ptr->convert_mem_size = __convert(_element_ptr->ptr, _element_ptr->str_size, &_element_ptr->convert_ptr);
			_element_ptr->convert_str_size = __strlen(_element_ptr->convert_ptr);
		}
		void						__clear_convert_str(ref_count_element_t* _element_ptr)
		{
			if (_element_ptr->convert_ptr)
			{
				__delete(_element_ptr->convert_ptr);
				_element_ptr->convert_ptr = nullptr;
				_element_ptr->convert_mem_size = 0;
				_element_ptr->convert_str_size = 0;
			}
		}
		template<typename t>
		void						__copy_str(t* _dst_ptr, const size_t _dst_mem_size, const t* _src_ptr, const size_t _src_mem_size, const size_t _src_size)
		{
			if (!_dst_ptr || !_dst_mem_size || !_src_ptr || !_src_mem_size || !_src_size)
				return;
			memmove_s(_dst_ptr, _dst_mem_size, _src_ptr, _src_mem_size);
			_dst_ptr[_src_size] = 0;
		}
		template<typename t>
		void						__copy_str(t* _dst_ptr, const size_t _dst_size, const t* _src_ptr, const size_t _src_size)
		{
			__copy_str(_dst_ptr, sizeof(t) * (_dst_size), _src_ptr, sizeof(t) * _src_size, _src_size);
		}
		template<typename t>
		void						__copy_str(t* _dst_ptr, const size_t _dst_size, const typename _convert_type<t>::type_t* _src_ptr, const size_t _src_size)
		{
			t* ptr = nullptr;
			__convert(_src_ptr, _src_size, &ptr);
			__copy_str(_dst_ptr, _dst_size, ptr, _src_size);
			__delete(ptr);
		}
		template<typename t>
		void						__assign(const t* _value_ptr, const size_t _size)
		{
			if (_size <= 0)
			{
				__release_string();
				return;
			}
			size_t mem_size = sizeof(element_t) * (_size + 1);
			if (!element_ptr_ || element_ptr_->count != 1 || mem_size > element_ptr_->mem_size)
			{
				__release_string();
				__new_element(&element_ptr_);
				__new_element_str(element_ptr_, _size, _value_ptr);
			}
			else
			{
				__copy_str(element_ptr_->ptr, _size, _value_ptr, _size);
				element_ptr_->str_size = _size;
				__clear_convert_str(element_ptr_);
			}
		}
		template<typename t>
		void						__assign(const t* _value_ptr)
		{
			if (_value_ptr) 
				assign(_value_ptr, __strlen(_value_ptr));
			else
				__release_string();
		}
		template<typename t>
		void						__append(const t* _value_ptr, const size_t _size)
		{
			if (_size <= 0)	
				return;
			bool is_reassign = false;
			size_t mem_size = sizeof(element_t) * (_size);
			size_t str_mem_size = sizeof(element_t) * (element_ptr_->str_size);
			if (!element_ptr_ || element_ptr_->count != 1 || mem_size + str_mem_size + sizeof(element_t) > element_ptr_->mem_size)
			{
				ref_count_element_t* new_element_ptr = nullptr;
				__new_element(&new_element_ptr);
				__new_element_str(new_element_ptr, _size + element_ptr_->str_size, (element_t*)nullptr);
				__copy_str(new_element_ptr->ptr, element_ptr_->str_size, element_ptr_->ptr, element_ptr_->str_size);
				__copy_str(new_element_ptr->ptr + element_ptr_->str_size, _size, _value_ptr, _size);
				__release_string();
				element_ptr_ = new_element_ptr;
			}
			else
			{
				__copy_str(element_ptr_->ptr + element_ptr_->str_size, _size, _value_ptr, _size);
				element_ptr_->str_size += _size;
				__clear_convert_str(element_ptr_);
			}
		}
		template<typename t>
		void						__append(const t* _value_ptr)
		{
			if (_value_ptr) 
				__append(_value_ptr, __strlen(_value_ptr));
			else
				__release_string();
		}
		template<typename t>
		void						__insert(const size_t _index, const t* _value_ptr, const size_t _size)
		{
			size_t real_index = _index;
			if (real_index > size())
				real_index = size();
			size_t mem_size = sizeof(element_t) * (_size);
			size_t str_mem_size = sizeof(element_t) * (element_ptr_->str_size);
			if (!element_ptr_ || element_ptr_->count != 1 || mem_size + str_mem_size + sizeof(element_t) > element_ptr_->mem_size)
			{
				ref_count_element_t* new_element_ptr = nullptr;
				__new_element(&new_element_ptr);
				__new_element_str(new_element_ptr, _size + element_ptr_->str_size, (element_t*)nullptr);
				__copy_str(new_element_ptr->ptr, real_index, element_ptr_->ptr, real_index);
				__copy_str(new_element_ptr->ptr + real_index, _size, _value_ptr, _size);
				__copy_str(new_element_ptr->ptr + real_index + _size, size() - real_index, element_ptr_->ptr + real_index, size() - real_index);
				__release_string();
				element_ptr_ = new_element_ptr;
			}
			else
			{
				element_t* begin_ptr = element_ptr_->ptr + real_index + _size;
				__copy_str(element_ptr_->ptr + real_index + _size, size() - real_index, element_ptr_->ptr + real_index, size() - real_index);
				element_t ch = 0;
				if (real_index != size())
					ch = *begin_ptr;
				__copy_str(element_ptr_->ptr + real_index, _size, _value_ptr, _size);
				*begin_ptr = ch;
				element_ptr_->str_size += _size;
				__clear_convert_str(element_ptr_);
			}
		}
		template<typename t>
		void						__insert(const size_t _index, const t* _value_ptr)
		{
			return __insert(_index, _value_ptr, __strlen(_value_ptr));
		}
		int							__find(const size_t _index, const element_t* _value_ptr, const size_t _size) const
		{
			if (!element_ptr_ || !element_ptr_->ptr || element_ptr_->str_size == 0 || _size == 0 || !_value_ptr)
				return -1;
			element_t* end = element_ptr_->ptr + size() - _size;
			int ret = 0;
			for (element_t* p = element_ptr_->ptr + _index; p <= end; ++p, ++ret)
			{
				size_t same = 0;
				for (; same < _size; ++same)
					if (p[same] != _value_ptr[same])
						break;
				if (same == _size)
					return ret;
			}
			return -1;
		}
		int							__find(const size_t _index, const convert_element_t* _value_ptr, const size_t _size) const
		{
			element_t* ptr = nullptr;
			_base_zn_string<element_t, allocate_t>* this_ptr = (_base_zn_string<element_t, allocate_t>*)this;
			this_ptr->__convert(_value_ptr, _size, &ptr);
			int ret = __find(_index, ptr, _size);
			this_ptr->__delete(ptr);
			return ret;
		}
		int							__replace(const size_t _index, const size_t _size, const element_t* _dst_ptr, const size_t _dst_size)
		{
			size_t new_size = size() - _size + _dst_size;
			if (_size > size())
				new_size = _dst_size;
			if (new_size == 0)
			{
				__release_string();
				return 0;
			}
			if (element_ptr_->count != 1 || sizeof(element_t) * (new_size + 1) > element_ptr_->mem_size)
			{
				ref_count_element_t* element_ptr;
				__new_element(&element_ptr);
				__new_element_str(element_ptr, new_size, (element_t*)nullptr);
				__copy_str(element_ptr->ptr, _index, element_ptr_->ptr, _index);
				__copy_str(element_ptr->ptr + _index, _dst_size, _dst_ptr, _dst_size);
				__copy_str(element_ptr->ptr + _index + _dst_size, element_ptr_->str_size - _index - _size, element_ptr_->ptr + _index + _size, element_ptr_->str_size - _index - _size);
				element_ptr_->ptr[new_size] = 0;
				__release_string();
				element_ptr_ = element_ptr;
			}
			else
			{
				__copy_str(element_ptr_->ptr + _index + _dst_size, element_ptr_->str_size - _index - _size, element_ptr_->ptr + _index + _size, element_ptr_->str_size - _index - _size);
				__copy_str(element_ptr_->ptr + _index, _dst_size, _dst_ptr, _dst_size);
				__clear_convert_str(element_ptr_);
				element_ptr_->ptr[new_size] = 0;
			}
			return (int)_index;
		}
		int							__replace(const size_t _index, const size_t _size, const convert_element_t* _dst_ptr, const size_t _dst_size)
		{
			element_t* ptr = nullptr;
			__convert(_dst_ptr, _dst_size, &ptr);
			int ret = __replace(_index, ptr, _dst_size);
			__delete(ptr);
		}
		template<typename t>
		int							__find_and_replace(const size_t _index, const t* _src_ptr, const size_t _src_size, const t* _dst_ptr, const size_t _dst_size)
		{
			int find_ret = find(_index, _src_ptr, _src_size);
			if (find_ret < 0) 
				return find_ret;
			if (_src_size == _dst_size && __strcmp(_src_ptr, _dst_ptr, _src_size) == 0)
				return find_ret;
			return __replace(find_ret, _src_size, _dst_ptr, _dst_size);
		}
		int							__fomat_print_s(char* _dst_ptr, const size_t _dst_size, const char* _format_ptr, va_list _arg_list)
		{
			return ::_vsnprintf_s(_dst_ptr, _dst_size, ((size_t)-1), _format_ptr, _arg_list);
		}
		int							__fomat_print_s(wchar_t* _dst_ptr, const size_t _dst_size, const wchar_t* _format_ptr, va_list _arg_list)
		{
			return ::_vsnwprintf_s(_dst_ptr, _dst_size, ((size_t)-1), _format_ptr, _arg_list);
		}
		int							__fomat_print(char* _dst_ptr, const size_t _dst_size, const char* _format_ptr, va_list _arg_list)
		{
			return ::_vsnprintf(_dst_ptr, _dst_size, _format_ptr, _arg_list);
		}
		int							__fomat_print(wchar_t* _dst_ptr, const size_t _dst_size, const wchar_t* _format_ptr, va_list _arg_list)
		{
			return ::_vsnwprintf(_dst_ptr, _dst_size, _format_ptr, _arg_list);
		}
	private:
		allocate_t					allocate_;
		ref_count_element_t*		element_ptr_;
	};

	typedef _base_zn_string<char, _base_zn_string_allocate>		zn_string;
	typedef _base_zn_string<wchar_t, _base_zn_string_allocate>	zn_wstring;
}

#pragma warning (pop)
#pragma warning (default : 4996)

#endif