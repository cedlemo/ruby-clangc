require "clangc/clangc"

module Clangc
  def self.visit_children(args)
    cursor = args[:cursor]
    callback = args[:visitor] || nil
    if(callback)
      visit_children_with_proc(cursor, callback)
    else
      visit_children_with_block(cursor) do |cursor, parent|
        yield(cursor, parent)
      end
    end
  end
  class TranslationUnit
    def diagnostics
      ds = []
      for i in 0..(diagnostics_num - 1) do
        ds << diagnostic(i)
      end
      ds
    end
  end
  class Diagnostic
    def source_ranges
      sr = []
      for i in 0..(num_ranges - 1) do
        sr << source_range(i)
      end
      sr
    end
  end
end
