module Binder
  NEWLINE ="\n"
  O_BRACKET ="("
  C_BRACKET = ")"
  O_CURLY_BRACKET = "{"
  C_CURLY_BRACKET = "}"
  COMMA = ","
  SEMI_COLON = ";"
  
  class OutputFiles
    attr_reader :_c, :_h
    def initialize(name)
      @_c = File.new((name||'wrapper') + '.c', 'w')
      @_h = File.new((name||'wrapper') + '.h', 'w')
      @_c.sync= true
      @_c.sync= true
    end
    def close_all
      @_c.close
      @_h.close
    end
  end 
end
