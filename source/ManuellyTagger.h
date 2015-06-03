#ifndef LOCALIZER_CAFFE_MANUELLCLASSIFIER_H
#define LOCALIZER_CAFFE_MANUELLCLASSIFIER_H


#include <string>
#include <deque>
#include <memory>

#include <boost/version.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/archive/basic_archive.hpp>
#include <boost/optional/optional.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <opencv2/core/core.hpp>
#include <QString>

#include <pipeline/datastructure/Tag.h>
#include <pipeline/Preprocessor.h>
#include <pipeline/Localizer.h>
#include <pipeline/EllipseFitter.h>

#include "Tag.h"
#include "Image.h"
#include "serialization.h"


namespace deeplocalizer {
namespace tagger {


class ManuellyTagger : public QObject {
    Q_OBJECT

public slots:
    void save(const QString & path) const;
    void loadNextImage();
    void loadLastImage();
    void loadImage(unsigned long idx);
signals:
    void image(ImageDescription * desc, Image * img);
    void outOfRange(unsigned long idx);
    void lastImage();
    void firstImage();
public:
    explicit ManuellyTagger();
    explicit ManuellyTagger(std::deque<ImageDescription> && descriptions);
    explicit ManuellyTagger(const std::vector<ImageDescription> & images_with_proposals);

    static std::unique_ptr<ManuellyTagger> load(const std::string & path);
    const std::deque<ImageDescription> & getProposalImages() const {
        return _image_descs;
    }

private:
    std::deque<ImageDescription> _image_descs;
    Image _image;
    ImageDescription * _desc;
    unsigned long _image_idx = 0;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize( Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(_image_descs);
        ar & BOOST_SERIALIZATION_NVP(_image_idx);
    }
};
}
}

#endif //LOCALIZER_CAFFE_MANUELLCLASSIFIER_H
