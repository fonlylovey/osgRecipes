#ifndef H_TRITONNODE
#define H_TRITONNODE

#include <osg/Geode>
#include <Triton.h>

namespace SilverLining
{
class Atmosphere;
}

class TritonNode : public osg::Geode
{
    class OceanDrawable : public osg::Drawable
    {
    public:
        virtual void drawImplementation( osg::RenderInfo& renderInfo ) const;
        virtual osg::BoundingBox computeBound() const;
        
        OceanDrawable( TritonNode* s=NULL );
        OceanDrawable( const OceanDrawable& copy, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY );
        META_Object( osg, OceanDrawable )
        
        struct OceanMeshData
        {
            osg::ref_ptr<osg::Vec3Array> vertices;
            osg::ref_ptr<osg::PrimitiveSet> primitiveSet;
        };
        
        std::vector<OceanMeshData>& getOceanMeshList() { return _oceanMeshes; }
        const std::vector<OceanMeshData>& getOceanMeshList() const { return _oceanMeshes; }
        
    protected:
        std::vector<OceanMeshData> _oceanMeshes;
        TritonNode* _triton;
    };
    
    struct OceanUpdater : public osg::NodeCallback
    {
    public:
        virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );
    };
    
public:
    TritonNode( const char* licenseUser=NULL, const char* licenseKey=NULL );
    TritonNode( const TritonNode& copy, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY );
    META_Node( osg, TritonNode )
    
    void addOceanMesh( osg::Vec3Array* vertices, osg::PrimitiveSet* primitiveSet );
    
    osg::Drawable* oceanDrawable() { return _oceanDrawable.get(); }
    const osg::Drawable* oceanDrawable() const { return _oceanDrawable.get(); }
    
    Triton::Environment* environment() { return _environment; }
    const Triton::Environment* environment() const { return _environment; }
    
    Triton::Ocean* ocean() { return _ocean; }
    const Triton::Ocean* ocean() const { return _ocean; }
    
    bool isOceanValid() const { return _initialized; }
    
    void setResourcePath( const std::string& path ) { _resourcePath = path; }
    const std::string& getResourcePath() const { return _resourcePath; }
    
    void setAtmosphere( SilverLining::Atmosphere* a ) { _atmosphere = a; }
    SilverLining::Atmosphere* atmosphere() { return _atmosphere; }
    const SilverLining::Atmosphere* atmosphere() const { return _atmosphere; }
    
    void setGlobalLight( osg::Light* l ) { _light = l; }
    osg::Light* getGlobalLight() { return _light.get(); }
    const osg::Light* getGlobalLight() const { return _light.get(); }
    
    // Called internally to start Triton environment
    virtual bool initializeTriton( osg::RenderInfo& renderInfo );
    
    // Called internally to update attached light according to SilverLining sky
    virtual void updateGlobalLight();
    
protected:
    virtual ~TritonNode();
    
    osg::observer_ptr<OceanDrawable> _oceanDrawable;
    osg::observer_ptr<osg::Light> _light;
    Triton::ResourceLoader* _resourceLoader;
    Triton::Environment* _environment;
    Triton::Ocean* _ocean;
    SilverLining::Atmosphere* _atmosphere;
    std::string _licenseName;
    std::string _licenseKey;
    std::string _resourcePath;
    bool _initialized;
};

#endif