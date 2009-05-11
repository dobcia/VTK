/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkLabeledDataMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkLabeledDataMapper - draw text labels at dataset points
// .SECTION Description
// vtkLabeledDataMapper is a mapper that renders text at dataset
// points. Various items can be labeled including point ids, scalars,
// vectors, normals, texture coordinates, tensors, and field data components.
//
// The format with which the label is drawn is specified using a
// printf style format string. The font attributes of the text can
// be set through the vtkTextProperty associated to this mapper. 
//
// By default, all the components of multi-component data such as
// vectors, normals, texture coordinates, tensors, and multi-component
// scalars are labeled. However, you can specify a single component if
// you prefer. (Note: the label format specifies the format to use for
// a single component. The label is creating by looping over all components
// and using the label format to render each component.)

// .SECTION Caveats
// Use this filter in combination with vtkSelectVisiblePoints if you want
// to label only points that are visible. If you want to label cells rather
// than points, use the filter vtkCellCenters to generate points at the
// center of the cells. Also, you can use the class vtkIdFilter to
// generate ids as scalars or field data, which can then be labeled.

// .SECTION See Also
// vtkMapper2D vtkActor2D vtkTextMapper vtkTextProperty vtkSelectVisiblePoints 
// vtkIdFilter vtkCellCenters

#ifndef __vtkLabeledDataMapper_h
#define __vtkLabeledDataMapper_h

#include "vtkMapper2D.h"

class vtkDataObject;
class vtkDataSet;
class vtkTextMapper;
class vtkTextProperty;
class vtkTransform;

#define VTK_LABEL_IDS        0
#define VTK_LABEL_SCALARS    1
#define VTK_LABEL_VECTORS    2
#define VTK_LABEL_NORMALS    3
#define VTK_LABEL_TCOORDS    4
#define VTK_LABEL_TENSORS    5
#define VTK_LABEL_FIELD_DATA 6

class VTK_RENDERING_EXPORT vtkLabeledDataMapper : public vtkMapper2D
{
public:
  // Description:
  // Instantiate object with %%-#6.3g label format. By default, point ids
  // are labeled.
  static vtkLabeledDataMapper *New();

  vtkTypeRevisionMacro(vtkLabeledDataMapper,vtkMapper2D);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Set/Get the format with which to print the labels.  This should
  // be a printf-style format string.
  //
  // By default, the mapper will try to print each component of the
  // tuple using a sane format: %d for integers, %f for floats, %g for
  // doubles, %ld for longs, et cetera.  If you need a different
  // format, set it here.  You can do things like limit the number of
  // significant digits, add prefixes/suffixes, basically anything
  // that printf can do.  If you only want to print one component of a
  // vector, see the ivar LabeledComponent.
  vtkSetStringMacro(LabelFormat);
  vtkGetStringMacro(LabelFormat);

  // Description:
  // Set/Get the component number to label if the data to print has
  // more than one component. For example, all the components of
  // scalars, vectors, normals, etc. are labeled by default
  // (LabeledComponent=(-1)). However, if this ivar is nonnegative,
  // then only the one component specified is labeled.
  vtkSetMacro(LabeledComponent,int);
  vtkGetMacro(LabeledComponent,int);

  // Description:
  // Set/Get the field data array to label. This instance variable is
  // only applicable if field data is labeled.  This will clear
  // FieldDataName when set.
  void SetFieldDataArray(int arrayIndex);
  vtkGetMacro(FieldDataArray,int);

  // Description:
  // Set/Get the name of the field data array to label.  This instance
  // variable is only applicable if field data is labeled.  This will
  // override FieldDataArray when set.
  void SetFieldDataName(const char *arrayName);
  vtkGetStringMacro(FieldDataName);

  // Description:
  // Set the input dataset to the mapper. This mapper handles any type of data.
  virtual void SetInput(vtkDataObject*);

  // Description:
  // Use GetInputDataObject() to get the input data object for composite
  // datasets.
  vtkDataSet *GetInput();

  // Description:
  // Specify which data to plot: scalars, vectors, normals, texture coords,
  // tensors, or field data. If the data has more than one component, use
  // the method SetLabeledComponent to control which components to plot.
  vtkSetMacro(LabelMode, int);
  vtkGetMacro(LabelMode, int);
  void SetLabelModeToLabelIds() {this->SetLabelMode(VTK_LABEL_IDS);};
  void SetLabelModeToLabelScalars() {this->SetLabelMode(VTK_LABEL_SCALARS);};
  void SetLabelModeToLabelVectors() {this->SetLabelMode(VTK_LABEL_VECTORS);};
  void SetLabelModeToLabelNormals() {this->SetLabelMode(VTK_LABEL_NORMALS);};
  void SetLabelModeToLabelTCoords() {this->SetLabelMode(VTK_LABEL_TCOORDS);};
  void SetLabelModeToLabelTensors() {this->SetLabelMode(VTK_LABEL_TENSORS);};
  void SetLabelModeToLabelFieldData()
            {this->SetLabelMode(VTK_LABEL_FIELD_DATA);};

  // Description:
  // Set/Get the text property.
  // If an integer argument is provided, you may provide different text
  // properties for different label types. The type is determined by an
  // optional type input array.
  virtual void SetLabelTextProperty(vtkTextProperty *p)
    { this->SetLabelTextProperty(p, 0); }
  virtual vtkTextProperty* GetLabelTextProperty()
    { return this->GetLabelTextProperty(0); }
  virtual void SetLabelTextProperty(vtkTextProperty *p, int type);
  virtual vtkTextProperty* GetLabelTextProperty(int type);

  // Description:
  // Draw the text to the screen at each input point.
  void RenderOpaqueGeometry(vtkViewport* viewport, vtkActor2D* actor);
  void RenderOverlay(vtkViewport* viewport, vtkActor2D* actor);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  virtual void ReleaseGraphicsResources(vtkWindow *);
  
  // Description:
  // The transform to apply to the labels before mapping to 2D.
  vtkGetObjectMacro(Transform, vtkTransform);
  void SetTransform(vtkTransform* t);

  //BTX
  /// Coordinate systems that output dataset may use.
  enum Coordinates
    {
    WORLD=0,           //!< Output 3-D world-space coordinates for each label anchor.
    DISPLAY=1          //!< Output 2-D display coordinates for each label anchor (3 components but only 2 are significant).
    };
  //ETX

  // Description:
  // Set/get the coordinate system used for output labels.
  // The output datasets may have point coordinates reported in the world space or display space.
  vtkGetMacro(CoordinateSystem,int);
  vtkSetClampMacro(CoordinateSystem,int,WORLD,DISPLAY);
  void CoordinateSystemWorld() { this->SetCoordinateSystem( vtkLabeledDataMapper::WORLD ); }
  void CoordinateSystemDisplay() { this->SetCoordinateSystem( vtkLabeledDataMapper::DISPLAY ); }

  // Description:
  // Return the modified time for this object.
  virtual unsigned long GetMTime();

protected:
  vtkLabeledDataMapper();
  ~vtkLabeledDataMapper();

  vtkDataSet *Input;

  char  *LabelFormat;
  int   LabelMode;
  int   LabeledComponent;
  int   FieldDataArray;
  char  *FieldDataName;
  int CoordinateSystem;

  vtkTimeStamp BuildTime;

  int NumberOfLabels;
  int NumberOfLabelsAllocated;
  vtkTextMapper **TextMappers;
  double* LabelPositions;
  vtkTransform *Transform;

  virtual int FillInputPortInformation(int, vtkInformation*);

  void AllocateLabels(int numLabels);
  void BuildLabels();
  void BuildLabelsInternal(vtkDataSet*);
  
  //BTX
  class Internals;
  Internals* Implementation;
  //ETX

private:
  vtkLabeledDataMapper(const vtkLabeledDataMapper&);  // Not implemented.
  void operator=(const vtkLabeledDataMapper&);  // Not implemented.
};

#endif

