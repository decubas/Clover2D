#ifndef BASE_BUFFER_H_
#define BASE_BUFFER_H_ 1


#include <Clover.h>

        struct VertexBufferElement
        {
            std::string name_;
            ShaderDataType type_;
            u32 size_;
            u32 offset_;

            bool normalized_;
            VertexBufferElement(){}
            VertexBufferElement(ShaderDataType type, CString name, bool normalized = false)
                : type_(type), name_(name), size_(ShaderDataTypeSize(type)), offset_(0), normalized_(normalized)
            {
            }

            u32 GetComponentCount() const
            {
                switch (type_)
                {
                case ShaderDataType::Float:  return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::UInt:   return 1;
                case ShaderDataType::Int:    return 1;
                case ShaderDataType::Int2:   return 2;
                case ShaderDataType::Int3:   return 3;
                case ShaderDataType::Int4:   return 4;
                case ShaderDataType::Mat3:   return 3 * 3;
                case ShaderDataType::Mat4:   return 4 * 4;
                case ShaderDataType::Bool:   return 1;
                }

                return 0;
            }
        };

        class VertexBufferLayout
        {
        public:
            VertexBufferLayout(){}
            VertexBufferLayout(const std::initializer_list<VertexBufferElement>& element)
                : elements_(element)
            {
                CalculateOffsetStride();
            }

            inline const CVector<VertexBufferElement>& getElements() const { return elements_; }
            u32 stride = 0;

			CVector<VertexBufferElement>::iterator begin() { return elements_.begin(); }
			CVector<VertexBufferElement>::iterator end() { return elements_.end(); }

        private:

            void CalculateOffsetStride()
            {
                u32 offset = 0;
                stride = 0;
                for (auto& element : elements_)
                {
                    element.offset_ = offset;
                    offset += element.size_;
                    stride += element.size_;
                }
            }
            
            
            CVector<VertexBufferElement> elements_;
        };

    
  class VertexBuffer : public Cl_Object
  {
  public:

      VertexBuffer() {};

    virtual void bind() const = 0;
    virtual void uploadData(
        const void *data, 
        unsigned int size, 
        unsigned int offset = 0) = 0;
    virtual void release() = 0;

    virtual void setLayout(const VertexBufferLayout& layout) = 0;
    virtual VertexBufferLayout& getLayout() = 0;

    unsigned int count() { return count_; }

    ~VertexBuffer() {};


  protected:
      VertexBufferLayout layout_;
      u32 attrib_index_;
      unsigned int count_;
      u32 render_id;

  };

  class IndexBuffer : public Cl_Object
  {
  public:
      
      IndexBuffer() {};
	  
      virtual void bind() const = 0;
      virtual void uploadData(
          const void* data,
          unsigned int size,
          unsigned int offset = 0) = 0;
      virtual void release() = 0;


      u32 count() { return count_; }
      u32 size() { return size_; }

      ~IndexBuffer() {};


  protected:
      u32 attrib_index_;
      u32 size_;
      u32 count_;
      u32 render_id;
  private:
  };


#endif
