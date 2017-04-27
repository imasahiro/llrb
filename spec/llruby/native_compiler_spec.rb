describe 'llruby::NativeCompiler' do
  def test_compile(&block)
    klass = Class.new
    klass.send(:define_singleton_method, :test, &block)
    result = klass.test
    expect(LLRuby::JIT.precompile(klass, :test)).to eq(true)
    expect(klass.test).to eq(result)
  end

  it 'compiles putnil' do
    test_compile { nil }
  end

  it 'compiles putobject' do
    test_compile { true }
    test_compile { false }
    test_compile { 100 }
  end
end