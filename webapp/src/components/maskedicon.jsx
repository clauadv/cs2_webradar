import { memo } from "react";

// Asset used as mask
// Omit label when decorative
const MaskedIcon = ({ path, height = 16, color = "bg-content-secondary", label, className = "" }) => {
  const mask = `url(${path}) no-repeat center / contain`;

  return (
    <div
      className={`${color} shrink-0 ${className}`}
      style={{
        WebkitMask: mask,
        mask,
        width: "auto",
        height,
      }}
      role={label ? "img" : "presentation"}
      aria-label={label || undefined}
      aria-hidden={label ? undefined : true}
    >
      {/* Sets intrinsic width, never seen */}
      <img className="h-full w-auto opacity-0" src={path} alt="" draggable={false} />
    </div>
  );
};

export default memo(MaskedIcon);
