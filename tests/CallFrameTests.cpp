
#include "gtest/gtest.h"

#include "windows.h"

#include "../common/CallFrame.h"


int parameterlessfuncValue = 0;

void STDMETHODCALLTYPE parameterlessfunc(){
    parameterlessfuncValue = 0;
}

TEST(CallFrameTests, Without_parameter) {
  CallFrame callFrame;

  parameterlessfuncValue = 123;
  
  callFrame.Call((FARPROC)parameterlessfunc);

  EXPECT_NE(parameterlessfuncValue, 123);
}

int STDMETHODCALLTYPE return_123_func(){
     return 123;
}

TEST(CallFrameTests, With_return_value) {
  CallFrame callFrame;

  auto value = callFrame.Call((FARPROC)return_123_func);

  EXPECT_EQ(value, 123);
}

int stack1value = 0;
float stack2value = 0.0f;

void STDMETHODCALLTYPE func_with_stack_params(int i1, int i2, int i3, int i4, int stack1, float stack2){
     stack1value = stack1;
     stack2value = stack2;
}

TEST(CallFrameTests, With_stack_params) {
  CallFrame callFrame;

  VARIANT value;

  value.intVal = 567;
  value.vt = VT_I2;
  
  // fill registers
  callFrame.pushWORD(value);
  callFrame.pushWORD(value);
  callFrame.pushWORD(value);
  callFrame.pushWORD(value);

  // fill stack
  value.intVal = 123;
  callFrame.pushWORD(value);

  value.vt = VT_R4;
  value.fltVal = 321.0f;
  callFrame.pushFloat(value);

  callFrame.Call((FARPROC)func_with_stack_params);

  EXPECT_EQ(stack1value, 123);
  EXPECT_EQ(stack2value, 321.0f);

  func_with_stack_params(765,765,765,765, 123, 321.0f);
}

template<typename TValue> 
class ParameterTestStruct{
public:
    static TValue Value;
    void static STDMETHODCALLTYPE parameterFunc(TValue value){
       Value = value;
    }
};

template<typename T> T ParameterTestStruct<T>::Value = 0;

TEST(CallFrameTests, With_byte_parameter) {
  CallFrame callFrame;
  
  VARIANT value;
  value.bVal = 123;
  value.vt = VT_I1;

  callFrame.pushByte(value);

  callFrame.Call((FARPROC)ParameterTestStruct<uint8_t>::parameterFunc);

  EXPECT_EQ(ParameterTestStruct<uint8_t>::Value, 123);
}

TEST(CallFrameTests, With_float_parameter) {
  CallFrame callFrame;
  
  VARIANT value;
  value.fltVal = 321.0f;
  value.vt = VT_R4;
  
  callFrame.pushFloat(value);

  callFrame.Call((FARPROC)ParameterTestStruct<float>::parameterFunc);

  EXPECT_EQ(ParameterTestStruct<float>::Value, 321.0f);
}


TEST(CallFrameTests, Resets_after_first_call) {
  CallFrame callFrame;
  
  VARIANT value;
  value.fltVal = 321.0f;
  value.vt = VT_R4;
  
  callFrame.pushFloat(value);

  callFrame.Call((FARPROC)ParameterTestStruct<float>::parameterFunc);
  
  EXPECT_EQ(ParameterTestStruct<float>::Value, value.fltVal);

  value.fltVal = 123.0f;
  callFrame.pushFloat(value);
  callFrame.Call((FARPROC)ParameterTestStruct<float>::parameterFunc);

  EXPECT_EQ(ParameterTestStruct<float>::Value, value.fltVal);
}