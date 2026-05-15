return {
  {
    "RedsXDD/neopywal.nvim",
    name = "neopywal",
    lazy = false,
    priority = 1000,
    opts = {},
  },
  -- Configure LazyVim to use neopywal as the default colorscheme
  {
    "LazyVim/LazyVim",
    opts = {
      colorscheme = "neopywal",
    },
  },
}
