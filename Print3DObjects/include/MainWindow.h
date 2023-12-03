#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <qinputdialog.h>
#include <qcolor.h>
#include <qbytearray.h>
#include <qvariant.h>
#include <QMessageBox.h>
#include <QVector>
#include <QDebug>
#include <QImage>
#include <QFileDialog>
#include <QString>
#include <QMouseEvent>
#include <QSlider>

#include <vtkSTLWriter.h>
#include <vtkSTLReader.h>

#include <QVTKRenderWidget.h>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkNew.h>
#include <vtkTransform.h>
#include <vtkPicker.h>
#include <vtkCellPicker.h>
#include <vtkPropPicker.h>
#include <vtkNamedColors.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkAppendPolyData.h>
#include <vtkProperty.h>
#include <vtkProp.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>
#include <vtkVersion.h>
#include <vtkCommand.h>


#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkPyramid.h>
#include <vtkTubeFilter.h>
#include <vtkLineSource.h>
#include <vtkParametricTorus.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricEllipsoid.h>
#include <vtkParametricSpline.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformPolyDataFilter.h>



#define NewPtr(obj, class) vtkSmartPointer<class> obj = vtkSmartPointer<class>::New()

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow* ui;
    static double azimuth, elevation;
    vtkNew<vtkNamedColors> colors;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer> mRenderer;
    vtkSmartPointer<QVTKInteractor> mInteractor;
    vtkSmartPointer<vtkInteractorStyle> mInteractorStyle;
    vtkSmartPointer<vtkInteractorStyleTrackballActor> mInteractorStyle1;
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInterActor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    QList<vtkSmartPointer<vtkActor>> actors;
    void updatePosition();
    void InitializeSlotsAndSignals();
    void InitializeBackground();
    void InitializeCameraSettings();
    void organizingWindowDependecies();
    vtkActor* pickActor();
private slots:
    void on_actionSave_triggered();
    void on_actionClose_triggered();
  
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onDrawSphereClick();
    void onDrawCubeClick();
    void onDrawCylinderClick();
    void onDrawConeClick();
    void onDrawPyramidClick();
    void onDrawTubeClick();
    void onDrawDoughnutClick();
    void onDrawCurvedCylinderClick();
    void onDrawSemiSphereClick();

    void updateCameraX(double value);
    void updateCameraY(double value);
    void updateCameraZ(double value);
    void updateCameraAzimuth(double value);
    void updateCameraElevation(double value);
    void newSketch();
    void deleteActor();
    void onOpacitySliderValueChanged(int value);
    void onMergeTwoObjects();

};

#endif // MAINWINDOW_H
