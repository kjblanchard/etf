local c = require("constants")
local a = {}

local animationData = {}

--TODO probably clean this up for better error handling
function a.Create(name, sprite)
  local s = Animator.CreateAnimator(name, animationData[name], sprite)
  return s
end

function a.CreateAnimatorData(name)
  return Animator.CreateAnimationData(name)
end

function a.PreloadAnimatorData()
  for _, name in ipairs(c.PreloadAnimators) do
    animationData[name] = a.CreateAnimatorData(name)
  end
end

---Plays a ui animation
---@param animator lightuserdata the animator
---@param name string anim name
---@param loops integer|nil how many loops, -1 is loop forever and is defaulted
function a.PlayAnimation(animator, name, loops)
  if not animator or not name then return end
  loops = loops or -1
  Animator.PlayAnimation(animator, name, loops)
end

return a
