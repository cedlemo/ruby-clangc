require "clangc/clangc"

module Clangc
  VERSION = "0.0.0"
  class TranslationUnit
    def diagnostics
      ds = []
      for i in 0..(diagnostics_num - 1) do
        ds << diagnostic(i)
      end
      ds
    end
  end
end
