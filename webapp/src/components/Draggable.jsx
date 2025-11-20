import { useEffect, useRef, useState } from "react";

const Draggable = ({ children, imgref, avrPing, radarContentRef, ...props }) => {
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const [isDragging, setIsDragging] = useState(false);
  const [dragOffset, setDragOffset] = useState({ x: 0, y: 0 });
  const draggableRef = useRef(null);
  const currentScale = imgref ? parseFloat(imgref.style.scale) || 1 : 1;
  let moveoverride = "false";
  let newtransx = "0";
  let newtransy = "0";
  if (imgref) { 
    moveoverride = imgref.getAttribute("moveoverride"); 
    newtransx = imgref.getAttribute("newtransx"); 
    newtransy = imgref.getAttribute("newtransy"); 
  }

  const handleMouseDown = (e) => {
    if (e.button !== 0 || moveoverride!="false") return;
    setIsDragging(true);
    setDragOffset({
      x: e.clientX - position.x * currentScale,
      y: e.clientY - position.y * currentScale,
    });
  };

  const handleMouseMove = (e) => {
    if (!isDragging) return;
    if (e.clientX - dragOffset.x < imgref.width*0.5 && e.clientX - dragOffset.x > imgref.width*-0.5 && e.clientY - dragOffset.y < imgref.height*0.5 && e.clientY - dragOffset.y > imgref.height*-0.5) {
      setPosition({
        x: (e.clientX - dragOffset.x) / currentScale,
        y: (e.clientY - dragOffset.y) / currentScale,
      });
    }
  };

  const handleMouseUp = () => {
    setIsDragging(false);
  };

  const handleMouseLeave = (e) => {
    if (isDragging && radarContentRef && !radarContentRef.contains(e.relatedTarget)) {
      setIsDragging(false);
    }
  };

  useEffect(()=>{
    if (moveoverride=="true") {
      setPosition({x: newtransx, y: newtransy})
    }
  }, [moveoverride, newtransx, newtransy])

  return (
    <div
      ref={draggableRef}
      onMouseDown={handleMouseDown}
      onMouseMove={handleMouseMove}
      onMouseUp={handleMouseUp}
      onMouseLeave={handleMouseLeave}
      style={{
        cursor: isDragging ? "grabbing" : "grab",
        userSelect: "none"
      }}
      {...props}
    >
      {children && typeof children === "object" && children.type
        ? {
            ...children,
            props: {
              ...children.props,
              isbeingdragged: `${isDragging}`,
              moveoverride: `${false}`,
              style: {
                ...children.props.style,
                transform: `translate(${position.x}px, ${position.y}px)`,
                transition: `transform ${avrPing}ms linear, scale 100ms linear`
              },
            },
          }
        : children}
    </div>
  );
};

export default Draggable;
