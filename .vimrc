" ============================================
" BASIC VIM CONFIGURATION
" ============================================

" ---------- Appearance ----------
syntax on                          " Enable syntax highlighting
set number                         " Show absolute line numbers
set relativenumber                 " Show relative line numbers
set cursorline                     " Highlight current line
set showcmd                        " Show command in bottom bar
set wildmenu                       " Visual autocomplete for commands
set mouse=a                        " Enable mouse support

" ---------- Indentation ----------
set tabstop=4                      " Number of spaces for a tab
set shiftwidth=4                   " Number of spaces for auto-indent
set expandtab                      " Use spaces instead of tabs
set autoindent                     " Copy indent from current line
set smartindent                    " Smart auto-indenting

" ---------- Search Options ----------
set hlsearch                       " Highlight search matches
set incsearch                      " Search as you type
set ignorecase                     " Case-insensitive search
set smartcase                      " Case-sensitive if uppercase present

" ---------- Files ----------
set nobackup                       " Don't create backup files
set noswapfile                     " Don't create swap files
set encoding=utf-8                 " Set default encoding

" ---------- Navigation ----------
set scrolloff=8                    " Keep 8 lines visible above/below cursor
set sidescrolloff=8                " Keep 8 columns visible left/right

" ---------- Status Line ----------
set laststatus=2                   " Always show status line
set statusline=%F%m%r%h%w\ [%Y]\ [%{&ff}]\ [%{&fileencoding}]\ %=%l,%c\ %p%%

" ---------- Search and Replace ----------
" Quick search and replace in current line
nnoremap <leader>s :s/

" Quick search and replace in entire file
nnoremap <leader>a :%s/

" ============================================
" COMMAND REFERENCE
" ============================================

" Search Commands:
"   /pattern     - Search forward
"   ?pattern     - Search backward
"   n            - Next match
"   N            - Previous match
"   *            - Search word under cursor (forward)
"   #            - Search word under cursor (backward)
"
" Search Options:
"   :set hlsearch    - Highlight matches (enabled)
"   :set nohlsearch  - Disable highlight
"   :nohlsearch      - Clear highlights temporarily
"
" Search and Replace:
"   :%s/old/new/g          - Replace all in file
"   :%s/old/new/gc         - Replace with confirmation
"   :s/old/new/g           - Replace in current line
"   :10,20s/old/new/g      - Replace in lines 10-20
"
" Navigation:
"   gg           - Go to top
"   G            - Go to bottom
"   :10          - Go to line 10
"   Ctrl-d       - Scroll down half page
"   Ctrl-u       - Scroll up half page
"   Ctrl-f       - Scroll down full page
"   Ctrl-b       - Scroll up full page
"
" Editing:
"   i            - Insert mode (before cursor)
"   a            - Insert mode (after cursor)
"   o            - New line below
"   O            - New line above
"   dd           - Delete line
"   yy           - Copy line
"   p            - Paste
"   u            - Undo
"   Ctrl-r       - Redo
"
" Visual Mode:
"   v            - Character selection
"   V            - Line selection
"   Ctrl-v       - Block selection

" ---------- Line and Block Movement ----------
" Normal Mode: Move single line up/down
nnoremap <C-j> :m +1<CR>==
nnoremap <C-k> :m -2<CR>==

" Visual Mode: Move highlighted block of lines up/down
vnoremap <C-j> :m '>+1<CR>gv=gv
vnoremap <C-k> :m '<-2<CR>gv=gv
