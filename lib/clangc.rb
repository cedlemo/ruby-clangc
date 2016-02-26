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
      num = num_ranges
      sr = []
      return sr if num == 0

      for i in 0..(num - 1) do
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
      num = num_arg_types
      return [] if num == -1

      types = []
      for i in 0..(num - 1) do
        types << arg_type(i)
      end
      types
    end
      ## 
    # :call-seq:
    # Clangc::Type#template_arguments_as_type -> Array
    #
    # Return an array that contains all the 
    # types for the arguments of the current Class Type template that
    # is related to the current cursor.
    # If the Cursor is not a Class Declaration, it returns
    # an empty array.
    def template_arguments_as_type
      num = num_template_arguments
      types = []
      return types if num == -1

      for i in 0..(num - 1) do
        types << template_argument_as_type(i)
      end
      types
    end
  end
  class Cursor 
    ## 
    # :call-seq:
    # Clangc::Cursor#arguments -> Array
    #
    # Return an array that contains all the 
    # cursors for the arguments of the function that
    # is related to the current cursor.
    # If the current cursor is not a function, it returns
    # an empty array.
    def arguments
      num = num_arguments
      cursors = []
      return cursors if num == -1

      for i in 0..(num - 1) do
        cursors << argument(i)
      end
      cursors
    end
    ## 
    # :call-seq:
    # Clangc::Cursor#overloaded_decls -> Array
    #
    # Return an array that contains all the 
    # cursors for the overloaded declarations that
    # are related to the current cursor.
    # If the current cursor is not an overloaded declaration, it returns
    # an empty array.
    def overloaded_decls
      num = num_overloaded_decls
      cursors = []
      return cursors if num == 0

      for i in 0..(num - 1) do
        cursors << overloaded_decl(i)
      end
      cursors
    end
    ## 
    # :call-seq:
    # Clangc::Cursor#template_arguments_kinds -> Array
    #
    # Return an array that contains all the 
    # kinds for the arguments of the function template that
    # is related to the current cursor.
    # If the current cursor is not a function declaration, it returns
    # an empty array.
    def template_arguments_kinds
      num = num_template_arguments
      kinds = []
      return kinds if num == -1

      for i in 0..(num - 1) do
        kinds << template_argument_kind(i)
      end
      kinds
    end
    ## 
    # :call-seq:
    # Clangc::Cursor#template_arguments_types -> Array
    #
    # Return an array that contains all the 
    # types for the arguments of the function template that
    # is related to the current cursor.
    # If the current cursor is not a function declaration, it returns
    # an empty array.
    def template_arguments_types
      num = num_template_arguments
      types = []
      return types if num == -1

      for i in 0..(num - 1) do
        types << template_argument_type(i)
      end
      types
    end
    ## 
    # :call-seq:
    # Clangc::Cursor#template_arguments_values -> Array
    #
    # Return an array that contains all the 
    # values for the arguments of the function template that
    # is related to the current cursor.
    # If the current cursor is not a function declaration, it returns
    # an empty array.
    def template_arguments_values
      num = num_template_arguments
      values = []
      return values if num == -1

      for i in 0..(num - 1) do
        values << template_argument_value(i)
      end
      values
    end
    ## 
    # :call-seq:
    # Clangc::Cursor#template_arguments_unsigned_values -> Array
    #
    # Return an array that contains all the 
    # values for the arguments of the function template that
    # is related to the current cursor.
    # If the current cursor is not a function declaration, it returns
    # an empty array.
    def template_arguments_unsigned_values
      num = num_template_arguments
      values = []
      return values if num == -1

      for i in 0..(num - 1) do
        values << template_argument_unsigned_value(i)
      end
      values
    end
  end
  class Module
    ## 
    # :call-seq:
    # Clangc::Module#top_level_headers(Clangc::TranslationUnit) -> Array
    #
    # Return an array that contains all the 
    # Clangc::File corresponding to the related
    # toplevel headers.
    # If the current cursor is not a module, it returns
    # an empty array.
    def top_level_headers(tu)
      num = num_top_level_headers(tu)
      headers = []
      return headers if num < 1

      for i in 0..(num - 1) do
        headers << top_level_header(tu, i)
      end
      headers
    end
  end
  class CompletionString
    ## 
    # :call-seq:
    # Clangc::CompletionString#chunk_kinds -> Array
    #
    # Return an array that contains all the 
    # kinds of the chunk completions for a
    # completion string.
    def chunk_kinds
      num = num_chunks
      return [] if num == -1

      kinds = []
      for i in 0..(num - 1) do
        kinds << chunk_kind(i)
      end
      kinds
    end
    ## 
    # :call-seq:
    # Clangc::CompletionString#chunk_texts -> Array
    #
    # Return an array that contains all the 
    # texts of the chunk completions for a
    # completion string.
    def chunk_texts
      num = num_chunks
      return [] if num == -1

      texts = []
      for i in 0..(num - 1) do
        texts << chunk_text(i)
      end
      texts
    end
    ## 
    # :call-seq:
    #   Clangc::CompletionString#num_annotations -> Array
    #
    # Return an array that contains all the 
    # annotations for a completion string.
    def annotations
      num = num_annotations
      return [] if num == -1

      annotations = []
      for i in 0..(num - 1) do
        annotations << annotation(i)
      end
      annotations
    end
  end
  class CodeCompleteResults
    ##
    # :call-seq:
    #   Clangc::CodeCompleteResults#results => Array
    #
    # Returns an Array of Clangc::CompletionResult
    def results
      num = num_results
      return [] if num < 0

      res = []
      for i in 0..(num - 1) do
        res << result(i)
      end
      res
    end
    ##
    # :call-seq:
    #   Clangc::CodeCompleteResults#diagnostics => Array
    #
    # Returns an Array of Clangc::Diagnostics
    def diagnostics
      num = num_diagnostics
      return [] if num < 0

      diags = []
      for i in 0..(num - 1) do
        diags << diagnostic(i)
      end
      diags
    end
  end
end
