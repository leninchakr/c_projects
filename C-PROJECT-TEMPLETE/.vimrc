" Display options
set number              " Show line numbers
set relativenumber      " Show relative line numbers
set cursorline          " Highlight current line
set showmatch           " Highlight matching brackets

" Search options
set hlsearch            " Highlight search results
set incsearch           " Show matches as you type
set ignorecase          " Case-insensitive search...
set smartcase           " ...unless query contains capital letters

" Indentation & Tabs
set tabstop=4           " 1 tab = 4 spaces
set shiftwidth=4        " Indent width for auto-indentation
set softtabstop=4       " Backspace treats 4 spaces like a tab
set expandtab           " Convert tabs to spaces
set autoindent          " Copy indent from current line when starting new line
set smartindent         " Auto-indent after opening braces '{'
set cindent             " Enable C-style indentation
" Force Linux Kernel style globally for ALL C and Header files
autocmd FileType c,cpp setlocal noexpandtab tabstop=8 shiftwidth=8 softtabstop=8
autocmd FileType c,cpp setlocal cinoptions={0,>s,e0,n0,f0,{0,g0,=s,p0,t0

" C/C++ Specific Alignment Rules
set cinoptions={0,>s,e0,n0,f0,{0,g0,=s,p0,t0 " Reset brace placement
set cinoptions+={0      " Place opening brace at 0 indent for function bodies
set cinoptions+=f0      " Place opening brace of a function at column 0

" Code Formatting & Alignment
filetype plugin indent on " Load filetype-specific indent rules

" System & UI
syntax on               " Enable syntax highlighting
set mouse=a             " Enable mouse support in terminal
set clipboard=unnamedplus " Use system clipboard (requires vim-gtk3)
set encoding=utf-8      " Set UTF-8 encoding
set backspace=indent,eol,start " Fix normal backspace behavior
