require "clangc/clangc"

module Clangc
  ##
  # :call-seq:
  # Clangc.visit_children(Hash)
  # 
  # This is a convenient method that call Clangc.visit_children_with_proc or
  # Clangc.visit_children_with_block.
  # the Hash arguments can accept two keys:
  # * cursor
  # * visitor
  #
  #    Clangc.visit_children(cursor: acursor, visitor: aproc)
  #
  #    Clangc.visit_children(cursor: acursor) do |cursor, parent|
  #      #do your stuf
  #      return Clangc::ChildVisitResult::Recurse
  #      #return Clangc::ChildVisitResult::Break
  #    end
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
    ## 
    # :call-seq:
    #   Clangc::TranslationUnit#diagnostics => Array
    # 
    # Returns an array of Clangc::Diagnostic for the current Clangc::TranslationUnit.
    # The array is empty if no Clangc::Diagnostic can be found.
    def diagnostics
      ds = []
      for i in 0..(diagnostics_num - 1) do
        ds << diagnostic(i)
      end
      ds
    end
  end
  class Diagnostic
    ## 
    # :call-seq:
    #   Clangc::Diagnostic#source_ranges => Array
    #
    # Returns an array of Clangc::SourceRange for the current Clangc::Diagnostic.
    # The array is empty if there is no Clangc::SourceRange
    def source_ranges
      num
      sr = []
      return sr if num == 0

      for i in 0..(num_ranges - 1) do
        sr << source_range(i)
      end
      sr
    end
  end
  class Type
  ## 
  # :call-seq:
  # Clangc::Type#arg_types -> Array
  #
  # Return an array that contains all the 
  # types for the argument of the function that
  # is related to the current type.
  # If the current type is not a function, it returns
  # an empty array.
  def arg_types
      num = num_args_types
      return [] if num == -1

      types = []
      for i in 0..(num - 1) do
        types << arg_type(i)
      end
      types
    end
  end
end
