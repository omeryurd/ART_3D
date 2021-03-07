var NAVTREE =
[
  [ "Monolith", "index.html", [
    [ "Monolith Overview", "index.html", null ],
    [ "Class List", "annotated.html", [
      [ "MTF::Camera", "class_m_t_f_1_1_camera.html", null ],
      [ "MTF::Display", "class_m_t_f_1_1_display.html", null ],
      [ "MTF::Eye", "class_m_t_f_1_1_eye.html", null ],
      [ "MTF::Head", "class_m_t_f_1_1_head.html", null ],
      [ "MTF::Matrix4", "class_m_t_f_1_1_matrix4.html", null ],
      [ "MTF::Monolith", "class_m_t_f_1_1_monolith.html", null ],
      [ "MTF::TrackerUpdate", "class_m_t_f_1_1_tracker_update.html", null ],
      [ "MTF::TrackingBody", "class_m_t_f_1_1_tracking_body.html", null ],
      [ "MTF::Vector3", "class_m_t_f_1_1_vector3.html", null ],
      [ "MTF::Wand", "class_m_t_f_1_1_wand.html", null ]
    ] ],
    [ "Class Index", "classes.html", null ],
    [ "Class Hierarchy", "hierarchy.html", [
      [ "MTF::Camera", "class_m_t_f_1_1_camera.html", null ],
      [ "MTF::Display", "class_m_t_f_1_1_display.html", null ],
      [ "MTF::Eye", "class_m_t_f_1_1_eye.html", null ],
      [ "MTF::Matrix4", "class_m_t_f_1_1_matrix4.html", null ],
      [ "MTF::Monolith", "class_m_t_f_1_1_monolith.html", null ],
      [ "MTF::TrackerUpdate", "class_m_t_f_1_1_tracker_update.html", null ],
      [ "MTF::TrackingBody", "class_m_t_f_1_1_tracking_body.html", [
        [ "MTF::Head", "class_m_t_f_1_1_head.html", null ],
        [ "MTF::Wand", "class_m_t_f_1_1_wand.html", null ]
      ] ],
      [ "MTF::Vector3", "class_m_t_f_1_1_vector3.html", null ]
    ] ],
    [ "Class Members", "functions.html", null ],
    [ "Namespace List", "namespaces.html", [
      [ "MTF", "namespace_m_t_f.html", null ]
    ] ],
    [ "Namespace Members", "namespacemembers.html", null ],
    [ "File List", "files.html", [
      [ "Camera.cpp", "_camera_8cpp.html", null ],
      [ "Camera.h", "_camera_8h.html", null ],
      [ "Display.cpp", "_display_8cpp.html", null ],
      [ "Display.h", "_display_8h.html", null ],
      [ "Eye.h", "_eye_8h.html", null ],
      [ "Head.cpp", "_head_8cpp.html", null ],
      [ "Head.h", "_head_8h.html", null ],
      [ "Matrix4.cpp", "_matrix4_8cpp.html", null ],
      [ "Matrix4.h", "_matrix4_8h.html", null ],
      [ "Monolith.cpp", "_monolith_8cpp.html", null ],
      [ "Monolith.h", "_monolith_8h.html", null ],
      [ "TrackerUpdate.cpp", "_tracker_update_8cpp.html", null ],
      [ "TrackerUpdate.h", "_tracker_update_8h.html", null ],
      [ "TrackingBody.h", "_tracking_body_8h.html", null ],
      [ "Vector3.cpp", "_vector3_8cpp.html", null ],
      [ "Vector3.h", "_vector3_8h.html", null ],
      [ "Wand.cpp", "_wand_8cpp.html", null ],
      [ "Wand.h", "_wand_8h.html", null ]
    ] ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

